#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "PS2_Mouse.h"

void PS2_Mouse_Init(void);
void PS2_Mouse_DeInit(void);
ErrorStatus PS2_Mouse_Start(void);
ErrorStatus PS2_Mouse_Timeout_TX ( volatile uint32_t nCount );
ErrorStatus PS2_Mouse_Timeout_RX ( volatile uint32_t nCount );
ErrorStatus PS2_Mouse_Tx(uint8_t cmd);
void PS2_Mouse_GetData(void);
void PS2_Mouse_CheckScancode(uint8_t scan);
void PS2_Mouse_ISR(void);


//	PA7 clk    timer
//	PC1 data	 gpio

volatile uint8_t scancode=0;
volatile uint8_t parity;

PS2_MOUSE_t 			PS2_MOUSE;
volatile PS2_MOUSE_VAR_t 	PS2_MOUSE_VAR;


void PS2_Mouse_Init(void){
	
	MOUSE_DATA_PORT->ODR|=DATA_ODR_ODRx;													
	MOUSE_DATA_PORT->MODER|=DATA_MODER_MODERx; 						 			// General purpose output mode
	MOUSE_DATA_PORT->OTYPER|=DATA_OTYPER_OTx;										// Output open-drain
	MOUSE_DATA_PORT->OSPEEDR|=DATA_OSPEEDER_OSPEEDRx;						// Medium speed
	MOUSE_DATA_PORT->PUPDR|=DATA_PUPDR_PUPDRx;									// Pull-up
	MOUSE_DATA_PORT->BSRR=DATA_BSRR_BS;													// линию  data  в :1
	
	MOUSE_CLOCK_PORT->ODR|=CLOCK_ODR_ODRx;													
	MOUSE_CLOCK_PORT->MODER|=CLOCK_MODER_MODERx; 						  // General purpose output mode
	MOUSE_CLOCK_PORT->OTYPER|=CLOCK_OTYPER_OTx;								// Output open-drain
	MOUSE_CLOCK_PORT->OSPEEDR|=CLOCK_OSPEEDER_OSPEEDRx;				// Medium speed
	MOUSE_CLOCK_PORT->PUPDR|=CLOCK_PUPDR_PUPDRx;							// Pull-up
	MOUSE_CLOCK_PORT->BSRR=CLOCK_BSRR_BS;																			// clock  в :1
	
	MOUSE_CLOCK_PORT->AFR|=GPIO_AFIO;							//  Настроим мультиплексор на AF2 TIM4. Порт пока в General purpose output mode
	
	
	__TIMx_CLK_ENABLE();
	TIMx->CR1|=TIM_CR1_CKD_1 ;								//tDTS = 4 × tCK_INT		
	TIMx->SMCR|=TIM_SMCR_TS_1|TIM_SMCR_TS_2;   //110: Filtered Timer Input 2 (TI2FP2)
	TIMx->SMCR|=TIM_SMCR_SMS;									//111: External Clock Mode 1 	
	TIMx->DIER|=TIM_DIER_TIE|TIM_DIER_UIE;		//<Update interrupt enable  Trigger interrupt enable. Прерывание по спаду и по переполнению TIM	
	TIMx->CCER|=TIM_CCER_CC2P;								// inverted/falling edge    По спаду линии clock
	
	TIMx->CCMR1|=TIM_CCMR1_IC2F_0|
								TIM_CCMR1_IC2F_2|
								TIM_CCMR1_IC2F_3;  				//  1101: fSAMPLING=fDTS/32, N=5   90MHz/4=22,5 MHz fDTS  Цифровой фильтр для PS2 clock.
	
	TIMx->CCMR1|=TIM_CCMR1_CC2S_0;						//	CC2 channel is configured as input, IC2 is mapped on TI2
	
	TIMx->PSC=0;
	TIMx->ARR=10;
	TIMx->EGR|=TIM_EGR_UG;										// Сделаем UEV чтобы ARR обновился
	TIMx->SR&=~TIM_SR_UIF; 										// Сбросим флаг прерывания по UEV
	HAL_NVIC_SetPriority(TIMx_IRQn, 0, 1);		// 
	NVIC_ClearPendingIRQ(TIMx_IRQn);					// 
	NVIC_EnableIRQ(TIMx_IRQn);								// Разрешим прерывание
	TIMx->CR1|=TIM_CR1_CEN;										// Запуск таймера
		
		PS2_MOUSE.status=MOUSE_INAKTIV;						// Начальные установки для мыши
  PS2_MOUSE.LButton=BTN_RELEASED;
  PS2_MOUSE.RButton=BTN_RELEASED;
  PS2_MOUSE.xPos=PS2_MOUSE_XPOS_START;
  PS2_MOUSE.yPos=PS2_MOUSE_YPOS_START;
  
  PS2_MOUSE_VAR.receive_code=0;							//	
  PS2_MOUSE_VAR.send_code=0;								// Обнуляем переменную для отправки и принятия данных. На всякий случай.
	
	
}
/*
*/
void PS2_Mouse_DeInit(void){
/* Сброс всех битов TIM в начальное состояние	*/
	
	TIMx->CR1&=~TIM_CR1_CEN;										// Выключаем таймер
	TIMx->CR1&=~TIM_CR1_CKD_1 ;									//tDTS = 4 × tCK_INT		
	TIMx->SMCR&=~TIM_SMCR_TS_1|TIM_SMCR_TS_2;   //110: Filtered Timer Input 2 (TI2FP2)
	TIMx->SMCR&=~TIM_SMCR_SMS;									//111: External Clock Mode 1 	
	TIMx->DIER&=~TIM_DIER_TIE|TIM_DIER_UIE;			//<Update interrupt enable  Trigger interrupt enable	
	TIMx->CCER&=~TIM_CCER_CC1P;									// inverted/falling edge
	TIMx->CCMR1&=~TIM_CCMR1_IC2F_0|
								TIM_CCMR1_IC2F_2|
								TIM_CCMR1_IC2F_3;  						//  1101: fSAMPLING=fDTS/32, N=5   90MHz/4=22,5 MHz fDTS  
	TIMx->CCMR1&=~TIM_CCMR1_CC2S_0;							//	CC2 channel is configured as input, IC2 is mapped on TI2
	
	TIMx->PSC=0;
	TIMx->ARR=0;
	HAL_NVIC_SetPriority(TIMx_IRQn, 0, 0);
	NVIC_ClearPendingIRQ(TIMx_IRQn);
	NVIC_DisableIRQ(TIMx_IRQn);
	__TIMx_CLK_DISABLE();																						// Выключаем тактирование таймера
	
	
																					
	MOUSE_DATA_PORT->MODER&=~DATA_MODER_MODERx;						//Input mode PB6 PB7
	MOUSE_DATA_PORT->OTYPER&=~DATA_OTYPER_OTx;						//Push-pull
	MOUSE_DATA_PORT->OSPEEDR&=~DATA_OSPEEDER_OSPEEDRx;		// Low speed
	MOUSE_DATA_PORT->PUPDR&=~DATA_PUPDR_PUPDRx;						//No-pull Выключам подтяжку
	
	MOUSE_CLOCK_PORT->AFR&=~GPIO_AFIO;											// AF2		
	MOUSE_CLOCK_PORT->MODER&=~CLOCK_MODER_MODERx;						//Input mode PB6 PB7
	MOUSE_CLOCK_PORT->OTYPER&=~CLOCK_OTYPER_OTx;						//Push-pull
	MOUSE_CLOCK_PORT->OSPEEDR&=~CLOCK_OSPEEDER_OSPEEDRx;		// Low speed
	MOUSE_CLOCK_PORT->PUPDR&=~CLOCK_PUPDR_PUPDRx;						//No-pull Выключам подтяжку
	
}
/*
*/
ErrorStatus PS2_Mouse_Start(void){

	PS2_MOUSE.type=MOUSE_TYP_UNKNOWN;
	
	//	Mouse Reset	
	PS2_Mouse_Tx(PS2_MOUSE_CMD_RESET);
	PS2_Mouse_Timeout_RX(20000);
	PS2_Mouse_Timeout_RX(20000);
		//  1. Sample-Frq = 200/s
	/*if(PS2_Mouse_Tx_TIMER(PS2_MOUSE_CMD_SAMPLE)==ERROR)
		return ERROR;
	if(PS2_Mouse_Tx_TIMER(200)==ERROR)
		return ERROR;
	//  2. Sample-Frq = 100/s
	if(PS2_Mouse_Tx_TIMER(PS2_MOUSE_CMD_SAMPLE)==ERROR)
		return ERROR;
	if(PS2_Mouse_Tx_TIMER(100)==ERROR)
		return ERROR;
	//  3. Sample-Frq = 80/s
	if(PS2_Mouse_Tx_TIMER(PS2_MOUSE_CMD_SAMPLE)==ERROR)
		return ERROR;
	if(PS2_Mouse_Tx_TIMER(80)==ERROR)
		return ERROR;*/
	 //  4. Maus-ID abfragen
	if(PS2_Mouse_Tx(PS2_MOUSE_CMD_ID)==ERROR)
		return ERROR;
	PS2_Mouse_Timeout_RX(20000);

	if(PS2_MOUSE_VAR.receive_code==0)
		PS2_MOUSE.type=MOUSE_TYP_STANDARD;
	if(PS2_MOUSE_VAR.receive_code==3)
		PS2_MOUSE.type=MOUSE_TYP_INTELLI;
	
	if(PS2_MOUSE.type!=MOUSE_TYP_UNKNOWN)
	{
		if(PS2_Mouse_Tx(PS2_MOUSE_CMD_SAMPLE)==ERROR)
			return ERROR;
		if(PS2_Mouse_Tx(60)==ERROR)
			return ERROR;
		
		/*if(PS2_Mouse_Tx_TIMER(PS2_MOUSE_CMD_SET_RESOLUTION)==ERROR)
			return ERROR;
		if(PS2_Mouse_Tx_TIMER(0X01)==ERROR)
			return ERROR;*/
		
		if(PS2_Mouse_Tx(PS2_MOUSE_CMD_ENABLE)==ERROR)
			return ERROR;
		if(PS2_Mouse_Tx(PS2_MOUSE_CMD_STREAM_MODE)==ERROR)
			return ERROR;
		PS2_MOUSE.status=MOUSE_AKTIV;
	}
	return SUCCESS;
}
/*
*/
ErrorStatus PS2_Mouse_Timeout_TX ( volatile uint32_t nCount ){
	
		ErrorStatus error = ERROR;
		while(nCount--)
		{
			if(PS2_MOUSE_VAR.mode==RX_MODE)
			{
				error=SUCCESS;
				break;
			}
		}
		return error;

}
/*
*/
ErrorStatus PS2_Mouse_Timeout_RX ( volatile uint32_t nCount ){

	ErrorStatus error = ERROR;
	PS2_MOUSE_VAR.receive_code=0;
	while ( nCount -- )
	{
		if ( PS2_MOUSE_VAR.receive_code!=0 )
		{
			error = SUCCESS;
			break;
		} 
	} 
	return error;
}

/*
*/
ErrorStatus PS2_Mouse_Tx(uint8_t cmd){
	
	uint16_t i;
	
	parity=0;
	PS2_MOUSE_VAR.send_code=cmd;
			
	for(i=0;i<8;i++)									//
	{																	//
		if(cmd&0x01)										//	Вычислим бит паритета таким примитивным способом.	
			parity++;											//	
		cmd>>=1;												//
	}
	if(parity%2)											//
		parity=0x0;											//
	else
		parity=0x01;										//
	
	
	//TIM->DIER&=~TIM_DIER_TIE;							//Запрет прерывания от TIM на время дерганья ножкой clock
	MOUSE_CLOCK_PORT->MODER&=~CLOCK_MODER_MODERx_1;
	MOUSE_CLOCK_PORT->MODER|=CLOCK_MODER_MODERx;  			//General purpose output mode
	
	MOUSE_CLOCK_PORT->BSRR=CLOCK_BSRR_BR;				//clock Low
	
	for(i=0;i<4000;i++);								// 20000 states (111 uSec)
	TIMx->CNT=0;												// Очень важно обнулять CNT перед отправкой след. команды. Не могу объяснить
																			// экспериментально вычислил.
	MOUSE_DATA_PORT->BSRR=DATA_BSRR_BR;				//data low
		
	MOUSE_CLOCK_PORT->BSRR=CLOCK_BSRR_BS;			//clock high
	
	
	MOUSE_CLOCK_PORT->MODER&=~CLOCK_MODER_MODERx;
	MOUSE_CLOCK_PORT->MODER|=CLOCK_MODER_MODERx_1; 				//Alternate function mode	
	//TIM->DIER|=TIM_DIER_TIE;							 // Разрешаем вновь прерывания для TIM
	
	PS2_MOUSE_VAR.receive_code=0;
	PS2_MOUSE_VAR.mode=TX_MODE;	
	
	if(PS2_Mouse_Timeout_TX(20000)==ERROR)					
		return ERROR;														
	if(PS2_Mouse_Timeout_RX(20000)==ERROR)
			return ERROR;
	if(PS2_MOUSE_VAR.receive_code!=PS2_MOUSE_CMD_ACK)
		return ERROR;
	return SUCCESS;
}
/*
*/
void PS2_Mouse_GetData(void){
		uint8_t temp;
	
	
/**************** Left Button **********/
		if((PS2_MOUSE_VAR.buff[0]&0x01)!=0)
		{
			PS2_MOUSE.LButton=BTN_PRESSED;
		}
		else
		{
			PS2_MOUSE.LButton=BTN_RELEASED;	
		}
/**************** Right Button **********/
		if((PS2_MOUSE_VAR.buff[0]&0x02)!=0)
		{
			PS2_MOUSE.RButton=BTN_PRESSED;
		}
		else
		{
			PS2_MOUSE.RButton=BTN_RELEASED;
		}
		
/**************** X Pos **********/		
		if((PS2_MOUSE_VAR.buff[0]&0x10)!=0)
		{
			temp=(256-PS2_MOUSE_VAR.buff[1]);
			if(PS2_MOUSE.xPos>=temp)
			{
				PS2_MOUSE.xPos-=temp;
			}
			/*else
			{
				PS2_MOUSE.xPos=0;
			}*/
		}
		else
		{
			temp=PS2_MOUSE_VAR.buff[1];
			if(PS2_MOUSE.xPos<=(479-temp))
				{
					PS2_MOUSE.xPos+=temp;
				}
				/*else
				{
					PS2_MOUSE.xPos=480;
				}*/
		}
/**************** Y Pos **********/		
		if((PS2_MOUSE_VAR.buff[0]&0x20)!=0)
		{
			temp=(256-PS2_MOUSE_VAR.buff[2]);
			if(PS2_MOUSE.yPos>=temp)
			{
				PS2_MOUSE.yPos-=temp;
			}
			/*else
			{
				PS2_MOUSE.yPos=0;
			}*/
		}
		else
		{
			temp=PS2_MOUSE_VAR.buff[2];
			if(PS2_MOUSE.yPos<=(271-temp))
				{
					PS2_MOUSE.yPos+=temp;
				}
			/*else
				{
					PS2_MOUSE.yPos=272;
				}*/
		}
/***********************************/		

}

/*
*/
void PS2_Mouse_CheckScancode(uint8_t scan){
	
	static uint8_t count_byte=0;
	
	PS2_MOUSE_VAR.receive_code=scan;
	if(PS2_MOUSE.status==MOUSE_AKTIV)
	{
		PS2_MOUSE_VAR.buff[count_byte]=scan;
		count_byte++;
		if(count_byte>=3)
		{
			count_byte=0;
			PS2_MOUSE.status=MOUSE_NEW_DATA;
		}
	
	}
	scancode=0;
}

/*
*/
void PS2_Mouse_ISR(void){
	uint8_t temp;
	
	temp=TIMx->CNT;
	
	if(PS2_MOUSE_VAR.mode==RX_MODE)//if(PS2_MOUSE.status==MOUSE_AKTIV)
	{
		if((temp>1)&&(temp<10))
		{
			scancode>>=1;
			if(MOUSE_DATA_PORT->IDR&DATA_IDR_IDRx) 
				scancode|=0x80;
		}
	}
	else if(PS2_MOUSE_VAR.mode==TX_MODE)
	{
		
		if(temp==1)
		{
		scancode=PS2_MOUSE_VAR.send_code;	
		if(scancode&0x01)
			MOUSE_DATA_PORT->BSRR=DATA_BSRR_BS;
		else
			MOUSE_DATA_PORT->BSRR=DATA_BSRR_BR;
		scancode>>=1;
		}
		else if((temp>1)&&(temp<9))
		{
			if(scancode&0x01)
				MOUSE_DATA_PORT->BSRR=DATA_BSRR_BS;
			else
				MOUSE_DATA_PORT->BSRR=DATA_BSRR_BR;
			scancode>>=1;
		}
		else if(temp==9)
		{
			if(parity)
				MOUSE_DATA_PORT->BSRR=DATA_BSRR_BS;
			else
				MOUSE_DATA_PORT->BSRR=DATA_BSRR_BR;
		
		}
		else if(temp==10)
		{
			MOUSE_DATA_PORT->BSRR=DATA_BSRR_BS;					//Stop bit
		}
	}
}
/*
*/
