#include <stm32f4xx.h>
#include "CAN.h"
#include "GUI.h"
CANTX_TypeDef CAN_Data_TX;
CANRX_TypeDef CAN_Data_RX[2];

#define ID_PROGBAR_1     	(GUI_ID_USER + 0x3C)
 
extern WM_HWIN hWin2;

extern volatile uint8_t message_flag;
extern volatile uint32_t countbyte_firmware;
extern volatile int size;
extern volatile uint8_t new_firmware;

extern void UART_Terminal_DMATran(char *p);
extern UART_HandleTypeDef huart1;

extern volatile uint8_t new_node;
extern WM_HWIN WinHandle[];
extern uint8_t backlight_count;

/****************************************************************************************************************
*														bxCAN_Init
****************************************************************************************************************/
void bxCAN_Init(void){

	GPIO_InitTypeDef GPIO_InitStruct;
		
	
	/*Включаем тактирование CAN в модуле RCC*/	
	RCC->APB1ENR|=RCC_APB1ENR_CAN2EN|RCC_APB1ENR_CAN1EN;
	/*Настройка выводов CAN  CAN2_TX=PA12   CAN2_RX=PA11  */
	
	GPIO_InitStruct.Alternate=GPIO_AF9_CAN2;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pin=GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	CAN2->RF1R|=CAN_RF0R_RFOM0;
	CAN2->RF1R|=CAN_RF1R_RFOM1;
	
	/*Настройка NVIC для bxCAN interrupt*/
	HAL_NVIC_SetPriority( CAN2_RX0_IRQn, 1, 0);
	HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 1, 0);
	HAL_NVIC_SetPriority(CAN2_SCE_IRQn, 1, 0);

//			Init mode				//

	//CAN2->MCR|=CAN_MCR_RESET;
	
	/*Exit SLEEP mode*/
	CAN1->MCR&=~CAN_MCR_SLEEP;
	CAN2->MCR&=~CAN_MCR_SLEEP;
	/*Enter Init mode bxCAN*/
	CAN1->MCR|=CAN_MCR_INRQ;
	CAN2->MCR|=CAN_MCR_INRQ;  /*Initialization Request */
	while((CAN2->MSR&CAN_MSR_INAK)!=CAN_MSR_INAK)		{}   /*while Initialization Acknowledge*/

	CAN2->MCR&=~CAN_MCR_DBF;			// CAN работает в режиме отладки//CAN останавливается в режиме отладки
	CAN2->MCR|=CAN_MCR_ABOM;
	CAN2->MCR&=~CAN_MCR_TTCM;
	CAN2->MCR&=~CAN_MCR_AWUM;
	CAN2->MCR&=~CAN_MCR_NART;		// автоматич. повторно передает	
	CAN2->MCR&=~CAN_MCR_RFLM;
	CAN2->MCR&=~CAN_MCR_TXFP;	
	/*Тестовый режиим работы выключен CAN  SILM=0  LBKM=0 */
	
	CAN2->BTR&=~CAN_BTR_LBKM;	
	CAN2->BTR&=~CAN_BTR_SILM;	

	CAN2->BTR|=CAN_BTR_BRP&29;														/* tq=(29+1)*tPCLK1=2/3 uS   */
	CAN2->BTR|=CAN_BTR_SJW_0;															/*SJW[1:0]=1  (SJW[1:0]+1)*tCAN=tRJW PROP_SEG =+- 2* tq	*/		
	
	//CAN2->BTR&=~CAN_BTR_TS1_0;
	CAN2->BTR|=CAN_BTR_TS1_2;															/* TS1[3:0]=0X07 */ //tBS1=tq*(7+1)=8tq
	
	//CAN2->BTR&=~CAN_BTR_TS2_1;	
	//CAN2->BTR|=CAN_BTR_TS2_0;				  									/* TS2[2:0]=0X02 */ //tBS2=tq*(2+1)=3tq
																												// | 1uS |  	8uS 					 |  3uS			| 		T=12*tq=12*2/3=8us f=125kHz
																												// |-------------------------|----------|		
																												//				Sample point = 75%
	/*Init filters*/
	CAN1->FMR|=	CAN_FMR_FINIT;																		// Filter Init Mode
		
	CAN1->FM1R&=~(CAN_FM1R_FBM14|CAN_FM1R_FBM15);												// Filters bank 14 15 mode ID mask
	CAN1->FM1R|=CAN_FM1R_FBM16;  																				// Filters bank 16  mode ID List
	CAN1->FS1R&=~(CAN_FS1R_FSC14|CAN_FS1R_FSC15|CAN_FS1R_FSC16);				// Filters bank 14 15 16  scale 16 bits
	CAN1->FFA1R&=~(CAN_FFA1R_FFA14|CAN_FFA1R_FFA15|CAN_FFA1R_FFA16);		// Filters bank 14 15 16  FIFO0		
		
	CAN1->FM1R|=CAN_FM1R_FBM17;																					// Filters bank 17      mode ID List		
	CAN1->FM1R&=~	(CAN_FM1R_FBM18|CAN_FM1R_FBM19);											// Filters bank 18  19   mode ID mask
	CAN1->FS1R&=~(CAN_FS1R_FSC17|CAN_FS1R_FSC18|CAN_FS1R_FSC19);				// Filters bank 17 18 19  scale 16 bits	
	CAN1->FFA1R|=CAN_FFA1R_FFA17|CAN_FFA1R_FFA18|CAN_FFA1R_FFA19;				// Filters bank 17 18 19  FIFO1		

	/*ID filters */
  //FOFO0
	CAN1->sFilterRegister[14].FR1=0x1FFF3000;	//Filters bank 14 fmi=00 ID=0xX80 IDE=0 RTR=0	фильтр для сообщ(0x180,0x280,0x380.)на запрос GET_RTC 0x080
																						//							 `  
	CAN1->sFilterRegister[14].FR2=0x30303020;	//Filters bank 14 fmi=01 ID=0x181 IDE=0 RTR=0	 
																						//		  				 `
	CAN1->sFilterRegister[15].FR1=0x1FFF3040;	//Filters bank 15 fmi=02 ID=0xX82 IDE=0 RTR=0	фильтр для сообщ(0x182,0x282,0x382.)на запрос GET_TIMER_DATA 0x082
																						//							 `
	CAN1->sFilterRegister[15].FR2=0x30703060;	//Filters bank 15 fmi=03 ID=0x183 IDE=0 RTR=0	 
																						//							 `fmi=04 ID=0x183 IDE=0 RTR=1
	CAN1->sFilterRegister[16].FR1=0x30903080;	//Filters bank 16 fmi=05 ID=0x184 IDE=0 RTR=0
																						//							 `fmi=06 ID=0x184 IDE=0 RTR=1
	CAN1->sFilterRegister[16].FR2=0x30B030A0;	//Filters bank 16 fmi 07 ID=0x185 IDE=0 RTR=0	
																						//							 `fmi=08 ID=0x185 IDE=0 RTR=1
	
	//FIFO1  
	CAN1->sFilterRegister[17].FR1=0x30D030C0;	//Filters bank 17 fmi=00 ID=0x186 IDE=0 RTR=0	
																						//								fmi=01 ID=0x186 IDE=0 RTR=1
	CAN1->sFilterRegister[17].FR2=0x30F030E0;	//Filters bank 17 fmi=02 ID=0x187 IDE=0 RTR=0	 
																						//								fmi=03 ID=0x187 IDE=0 RTR=1
	CAN1->sFilterRegister[18].FR1=0x1FFF3100;	//Filters bank 18 fmi=04 ID=0xX88 IDE=0 RTR=0	фильтр для сообщ(0x188,0x288,0x388..)на запрос GET_NETNAME 0x088
																						//								
	CAN1->sFilterRegister[18].FR2=0x1FFF4E40;	//Filters bank 18 fmi=05 ID=0xX72 IDE=0 RTR=0	фильтр для сообщ(0x172,0x272,0x372...) на запрос 0xX71 UPDATE_FIRMWARE_REQ 
																						// 								
	CAN1->sFilterRegister[19].FR1=0x1FFF4E80;	//Filters bank 19 fmi=06 ID=0xX74 IDE=0 RTR=0	фильтр для 0xX74 (0x174) 0x274 0x374) запрос от bootloader GET_FIRMWARE	
																						//								
	CAN1->sFilterRegister[19].FR2=0x31703160;	//Filters bank 19 fmi=08 ID=0x18B IDE=0 RTR=0
																						// 								fmi=09 ID=0x18B IDE=0 RTR=1	
	/* Filters activation  */	
	CAN1->FA1R|=CAN_FFA1R_FFA14|CAN_FFA1R_FFA15|CAN_FFA1R_FFA16|
							CAN_FFA1R_FFA17|CAN_FFA1R_FFA18|CAN_FFA1R_FFA19;		
							
	/*Exit filters init mode*/
	CAN1->FMR&=	~CAN_FMR_FINIT;
	
	/*Разрешение прерываний FIFO0 FIFO1*/
	CAN2->IER|=CAN_IER_FMPIE0|CAN_IER_FMPIE1;
	CAN2->IER|=CAN_IER_ERRIE;
//	 Exit Init mode bxCAN	

	CAN2->MCR&=~CAN_MCR_INRQ;  														/*Initialization Request */	
	while((CAN2->MSR&CAN_MSR_INAK)==CAN_MSR_INAK)		{}   /*while Initialization Acknowledge*/		

	HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);		
	HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);	
}
/*****************************************************************************************************************
*													CAN_Transmit_DataFrame
******************************************************************************************************************/
CAN_TXSTATUS CAN_Transmit_DataFrame(CANTX_TypeDef *Data){
		uint32_t temp=0;
		uint8_t mailbox_index;
	
	if((CAN2->TSR&CAN_TSR_TME0)==CAN_TSR_TME0)
		mailbox_index=0;
	else if((CAN2->TSR&CAN_TSR_TME1)==CAN_TSR_TME1)
		mailbox_index=1;
	else if((CAN2->TSR&CAN_TSR_TME2)==CAN_TSR_TME2)
		mailbox_index=2;
	else
		return CAN_TXBUSY;
	
	//temp=(Data->ID<<3)|(0x4);
	CAN2->sTxMailBox[mailbox_index].TIR=(uint32_t)(Data->ID<<21);
	
	CAN2->sTxMailBox[mailbox_index].TDTR&=(uint32_t)0xfffffff0;
	CAN2->sTxMailBox[mailbox_index].TDTR|=Data->DLC;
	
	temp=(Data->Data[3]<<24)|(Data->Data[2]<<16)|(Data->Data[1]<<8)|(Data->Data[0]);
	CAN2->sTxMailBox[mailbox_index].TDLR=temp;
	temp=(Data->Data[7]<<24)|(Data->Data[6]<<16)|(Data->Data[5]<<8)|(Data->Data[4]);
	CAN2->sTxMailBox[mailbox_index].TDHR=temp;
	
	/*Send message*/
	CAN2->sTxMailBox[mailbox_index].TIR|=CAN_TI0R_TXRQ;
	return CAN_TXOK;
		
}	
/*****************************************************************************************************************
*													CAN_Transmit_RemoteFrame
******************************************************************************************************************/

CAN_TXSTATUS CAN_Transmit_RemoteFrame(uint16_t ID){
	
	uint8_t mailbox_index;
	
	if((CAN2->TSR&CAN_TSR_TME0)==CAN_TSR_TME0)
		mailbox_index=0;
	else if((CAN2->TSR&CAN_TSR_TME1)==CAN_TSR_TME1)
		mailbox_index=1;
	else if((CAN2->TSR&CAN_TSR_TME2)==CAN_TSR_TME2)
		mailbox_index=2;
	else
		return CAN_TXBUSY;
	
	//temp=(ID<<3)|(0x6);
	CAN2->sTxMailBox[mailbox_index].TIR=(uint32_t)((ID<<21)|0x2);
	CAN2->sTxMailBox[mailbox_index].TDTR&=(uint32_t)0xfffffff0;
	
	/*Send message*/
	CAN2->sTxMailBox[mailbox_index].TIR|=CAN_TI0R_TXRQ;
	return CAN_TXOK;
}
/*****************************************************************************************************************
*													CAN_Receive
******************************************************************************************************************/
void CAN_Receive_IRQHandler(uint8_t FIFONumber){
	
	if((CAN2->sFIFOMailBox[FIFONumber].RIR&CAN_RI0R_RTR)!=CAN_RI0R_RTR)
	{
		
		CAN_Data_RX[FIFONumber].Data[0]=(uint8_t)0xFF&(CAN2->sFIFOMailBox[FIFONumber].RDLR);
		CAN_Data_RX[FIFONumber].Data[1]=(uint8_t)0xFF&(CAN2->sFIFOMailBox[FIFONumber].RDLR>>8);
		CAN_Data_RX[FIFONumber].Data[2]=(uint8_t)0xFF&(CAN2->sFIFOMailBox[FIFONumber].RDLR>>16);
		CAN_Data_RX[FIFONumber].Data[3]=(uint8_t)0xFF&(CAN2->sFIFOMailBox[FIFONumber].RDLR>>24);
		
		CAN_Data_RX[FIFONumber].Data[4]=(uint8_t)0xFF&(CAN2->sFIFOMailBox[FIFONumber].RDHR);
		CAN_Data_RX[FIFONumber].Data[5]=(uint8_t)0xFF&(CAN2->sFIFOMailBox[FIFONumber].RDHR>>8);
		CAN_Data_RX[FIFONumber].Data[6]=(uint8_t)0xFF&(CAN2->sFIFOMailBox[FIFONumber].RDHR>>16);
		CAN_Data_RX[FIFONumber].Data[7]=(uint8_t)0xFF&(CAN2->sFIFOMailBox[FIFONumber].RDHR>>24);
		
		CAN_Data_RX[FIFONumber].DLC=(uint8_t)0x0F & CAN2->sFIFOMailBox[FIFONumber].RDTR;
		
	}
		CAN_Data_RX[FIFONumber].ID=(uint16_t)0x7FF & (CAN2->sFIFOMailBox[FIFONumber].RIR>>21);
	
		CAN_Data_RX[FIFONumber].FMI=(uint8_t)0xFF & (CAN2->sFIFOMailBox[FIFONumber].RDTR>>8);
	
	
		
	/*Release FIFO*/
	
	if(FIFONumber)
	{
		CAN2->IER&=~CAN_IER_FMPIE1;
		CAN2->RF1R|=CAN_RF1R_RFOM1;	///*Запрет прерываний  FIFO1 на время обработки сообщения*/
	}
	else	
	{
		CAN2->RF0R|=CAN_RF0R_RFOM0;
		CAN2->IER&=~CAN_IER_FMPIE0;  ///*Запрет прерываний FIFO0  на время обработки сообщения*/
	}
}

/*
*
*/
/*****************************************************************************************************************
*													CAN_RXProcess0
******************************************************************************************************************/
void CAN_RXProcess0(void){
	CANNode_TypeDef CANNode;
	uint8_t index;
	index=CAN_Data_RX[0].Data[0];
	switch(CAN_Data_RX[0].FMI) {
		case 0://(id=x80 data get rtc-time_date)
		//
		if(WinHandle[index])
		{
			FRAMEWIN_GetUserData(WinHandle[index],&CANNode,sizeof(CANNode));
			CANNode.time[0]=CAN_Data_RX[0].Data[1];
			CANNode.time[1]=CAN_Data_RX[0].Data[2];
			CANNode.date[0]=CAN_Data_RX[0].Data[3];
			CANNode.date[1]=CAN_Data_RX[0].Data[4];
			CANNode.date[2]=CAN_Data_RX[0].Data[5];
			CANNode.newmsg=1;
			FRAMEWIN_SetUserData(WinHandle[index],&CANNode,sizeof(CANNode));
			/*CANNode_Struct[CAN_Data_RX[0].Data[0]].time[0]=CAN_Data_RX[0].Data[1];
			CANNode_Struct[CAN_Data_RX[0].Data[0]].time[1]=CAN_Data_RX[0].Data[2];
			CANNode_Struct[CAN_Data_RX[0].Data[0]].date[0]=CAN_Data_RX[0].Data[3];
			CANNode_Struct[CAN_Data_RX[0].Data[0]].date[1]=CAN_Data_RX[0].Data[4];
			CANNode_Struct[CAN_Data_RX[0].Data[0]].date[2]=CAN_Data_RX[0].Data[5];
			CANNode_Struct[CAN_Data_RX[0].Data[0]].newmsg=1;*/
		}
			backlight_count=0;
			TIM7->CNT=0;
			TIM13->CCR1=70;
			if((TIM7->CR1&TIM_CR1_CEN)!=TIM_CR1_CEN)
				TIM7->CR1|=TIM_CR1_CEN;
		break;
		case 1://(id=181 data set_rtc)
		
		break;
		//
		case 2://(id=x82 data get_timer)
		//
		if(WinHandle[index])
		{
			FRAMEWIN_GetUserData(WinHandle[index],&CANNode,sizeof(CANNode));
			CANNode.can_err[0]=CAN_Data_RX[0].Data[7];  //CAN_REC
			CANNode.can_err[1]=CAN_Data_RX[0].Data[6];	//CAN_TEC
			CANNode.can_err[2]=CAN_Data_RX[0].Data[5];	//CAN_ERF
			CANNode.timer0nOff=CAN_Data_RX[0].Data[4];
			CANNode.timerBrez_value=CAN_Data_RX[0].Data[3];
			CANNode.timerPhase_value=CAN_Data_RX[0].Data[2];
			CANNode.timerPhaseBrez=CAN_Data_RX[0].Data[1];
			CANNode.newmsg=1;
			FRAMEWIN_SetUserData(WinHandle[index],&CANNode,sizeof(CANNode));
		}
		/*CANNode_Struct[CAN_Data_RX[0].Data[0]].can_err[0]=CAN_Data_RX[0].Data[7];  //CAN_REC
		CANNode_Struct[CAN_Data_RX[0].Data[0]].can_err[1]=CAN_Data_RX[0].Data[6];	//CAN_TEC
		CANNode_Struct[CAN_Data_RX[0].Data[0]].can_err[2]=CAN_Data_RX[0].Data[5];	//CAN_ERF
		CANNode_Struct[CAN_Data_RX[0].Data[0]].timer0nOff=CAN_Data_RX[0].Data[4];
		CANNode_Struct[CAN_Data_RX[0].Data[0]].timerBrez_value=CAN_Data_RX[0].Data[3];
		CANNode_Struct[CAN_Data_RX[0].Data[0]].timerPhase_value=CAN_Data_RX[0].Data[2];
		CANNode_Struct[CAN_Data_RX[0].Data[0]].timerPhaseBrez=CAN_Data_RX[0].Data[1];
		CANNode_Struct[CAN_Data_RX[0].Data[0]].newmsg=1;*/
		backlight_count=0;
		TIM7->CNT=0;
		TIM13->CCR1=70;
		if((TIM7->CR1&TIM_CR1_CEN)!=TIM_CR1_CEN)
			TIM7->CR1|=TIM_CR1_CEN;
		break;
		case 3://(id=183 data set_timer)
		//
		
		break;
		
		default:
		break;	
	
	}

	/*Разрешение прерываний FIFO0 */
	CAN2->IER|=CAN_IER_FMPIE0;
	

}	

/*****************************************************************************************************************
*													CAN_RXProcess1
******************************************************************************************************************/
void CAN_RXProcess1(void){
	uint8_t index;
	uint32_t i,temp;
	PROGBAR_Handle progbar;
	
	switch(CAN_Data_RX[1].FMI) {
		case 4://(id=x88 data get netname ответы с net name)
			netname_index=CAN_Data_RX[1].Data[0]; 	
			new_node=1;
		break;
		
		case 5:	//(id=0xX72 ) ответы на запрос UPDATE_FIRMWARE_REQ
			index=CAN_Data_RX[1].Data[0];
		
			progbar=WM_GetDialogItem(hWin2,ID_PROGBAR_1);
			if(CAN_Data_RX[1].Data[1]=='g')		// получили в сообщении 'g' GET_DATA!
			{								
				if((size-countbyte_firmware)>=8)
				{
					for(i=0;i<8;i++)
						{
							CAN_Data_TX.Data[i]=*(uint8_t*)(0xD0400000+countbyte_firmware);
							countbyte_firmware++;
						}
					CAN_Data_TX.ID=((index+1)<<8)|0x73;
					CAN_Data_TX.DLC=8;	
					CAN_Transmit_DataFrame(&CAN_Data_TX);
						
					temp=(countbyte_firmware*100)/size;
					//*(uint8_t*)(Download_bytes+9)=(temp/10)|0x30;
					//*(uint8_t*)(Download_bytes+10)=(temp%10)|0x30;		
					//__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
					//UART_Terminal_DMATran(Download_bytes);
					
					PROGBAR_SetValue(progbar,temp);
				}
				else
				{
					CAN_Data_TX.DLC=(uint8_t)(size-countbyte_firmware);	
					for(i=0;i<CAN_Data_TX.DLC;i++)
						{
							CAN_Data_TX.Data[i]=*(uint8_t*)(0xD0400000+countbyte_firmware);
							countbyte_firmware++;
						}
					CAN_Data_TX.ID=((index+1)<<8)|0x73;
					CAN_Transmit_DataFrame(&CAN_Data_TX);
				}
			}
			else if(CAN_Data_RX[1].Data[1]=='c')		// получили в сообщении 'c' CRC OK!
			{
				countbyte_firmware=0;
				new_firmware=0;
				message_flag=1;
			}
			else if(CAN_Data_RX[1].Data[1]=='e')		// получили в сообщении 'e' CRC ERROR!
			{
				countbyte_firmware=0;
				new_firmware=0;
				message_flag=2;
			}
			else if(CAN_Data_RX[1].Data[1]=='s')		// получили в сообщении 's' SIZE ERROR!
			{
				countbyte_firmware=0;
				new_firmware=0;
				message_flag=3;
			}		
		break;	
		
		case 6://(id=x74 get_firmware)
			netname_index=CAN_Data_RX[1].Data[0];
			if(netname_index==new_firmware)
			{
				progbar=WM_GetDialogItem(hWin2,ID_PROGBAR_1);
				WM_ShowWindow(progbar);
				
				countbyte_firmware=0;
				CAN_Data_TX.ID=((netname_index+1)<<8)|0x71;
				CAN_Data_TX.DLC=4;
				CAN_Data_TX.Data[0]=(uint8_t)size;
				CAN_Data_TX.Data[1]=(uint8_t)(size>>8);
				CAN_Data_TX.Data[2]=(uint8_t)(size>>16);
				CAN_Data_TX.Data[3]=(uint8_t)(size>>24);
				CAN_Transmit_DataFrame(&CAN_Data_TX);				// (Core4X9I 0x271) UPDATE_FIRMWARE_REQ 0x271, 0x371, 0x471
			}
		break;	
		default:
		break;	
	}
	/*Разрешение прерываний FIFO1*/
	CAN2->IER|=CAN_IER_FMPIE1;
}

