/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* USER CODE BEGIN 0 */

#include "GUI.h"
#include "DIALOG.h"
#include "PS2_Mouse.h"
#include "CAN.h"


extern void UART_Terminal_DMATran(char *p);
extern char Download_bytes[];
extern volatile uint32_t countbyte_firmware;

extern  uint8_t RxBuffer[];
extern  uint8_t TxBuffer[];
extern volatile uint8_t uart_get_size;
extern volatile uint8_t uart_get_data;
extern volatile uint8_t uart_end_transaction;
extern volatile uint8_t uart_data_message;
extern  volatile uint8_t uart_newmessage;

volatile uint8_t hwin=0;
extern volatile uint8_t change_Layer;
volatile WM_HWIN current_widget;
volatile WM_HWIN last_widget;
//const char str3[]="ok";
//extern char str2[];
//extern volatile uint8_t tx_message;
//extern volatile uint8_t rx_message;

//static volatile uint8_t RXcount,TXcount;
//extern volatile uint8_t count_message;
extern volatile int size;
extern volatile int transaction;
extern volatile GUI_TIMER_TIME OS_TimeMS;
volatile uint32_t TimeMS;
extern void Touch_GetData(void);
extern CANRX_TypeDef CAN_Data_RX;

volatile uint8_t timeupdate_flag;
extern uint8_t lcd_bright;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;

extern GUI_PID_STATE State;
extern PROGBAR_Handle PROGBAR_MEM;
uint32_t GUI_UsedBytes;
uint8_t backlight_count=0;


extern PS2_MOUSE_t 		PS2_MOUSE;
extern volatile uint8_t scancode;
extern volatile PS2_MOUSE_VAR_t 	PS2_MOUSE_VAR;
extern uint8_t Input_Device; 

extern uint32_t ALARMA_ACTION;
extern uint32_t ALARMB_ACTION;

extern WM_HWIN hWin2;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern LTDC_HandleTypeDef hltdc;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef  hdma2;
/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
	OS_TimeMS++;
  TimeMS++;
	/* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles RTC wake-up interrupt through EXTI line 22.
*/
void RTC_WKUP_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_WKUP_IRQn 0 */
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		timeupdate_flag=1;
  /* USER CODE END RTC_WKUP_IRQn 0 */
  HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_WKUP_IRQn 1 */
		
  /* USER CODE END RTC_WKUP_IRQn 1 */
}
 void USART1_IRQHandler(void){
	/* USER CODE BEGIN USART1_IRQn 0 */
	 uint32_t temp;
	 
	 if(uart_data_message)
		{											// если принимаем данные (data)
			
				if(DMA2_Stream2->NDTR>16)
				{
					uart_get_data=1;
						
				}
				else
				{
					uart_get_size=1;
				}
		}
		else
		{															// если принимаем сообщение	(message)
			uart_newmessage=1;					//  выставляем флаг нового сообщения	
					
		}
		// Сбросим бит IDLE след последовательностью
		temp=USART1->SR;
		temp=USART1->DR;
		
	/* USER CODE END USART1_IRQn 0 */
	//HAL_UART_IRQHandler(&huart1);
	/* USER CODE BEGIN USART1_IRQn 1 */
	
  /* USER CODE END USART1_IRQn 1 */ 
 
 }


/**
* @brief This function handles RTC alarms A and B interrupt through EXTI line 17.
*/
void RTC_Alarm_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_Alarm_IRQn 0 */
	if((RTC->ISR&RTC_ISR_ALRAF)==RTC_ISR_ALRAF)	
	{	
		RTC->ISR&=~RTC_ISR_ALRAF;
		switch(ALARMA_ACTION){
			case NONE:
			break;	
			case START_PWM:
			__nop();
			break;
			case STOP_PWM:
			break;
			case START_ADC:
			break;
			case STOP_ADC:
			break;
			default:
			break;		
		}
	}
	else
	{
		RTC->ISR&=~RTC_ISR_ALRBF;
		switch(ALARMB_ACTION){
			case NONE:
			break;
			case START_PWM:
			break;
			case STOP_PWM:
			__nop();
			break;
			case START_ADC:
			break;
			case STOP_ADC:
			break;
			default:
			break;	
			}
	}
  /* USER CODE END RTC_Alarm_IRQn 0 */
  HAL_RTC_AlarmIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_Alarm_IRQn 1 */

  /* USER CODE END RTC_Alarm_IRQn 1 */
}

/**
* @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
*/
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
		uint32_t temp;
	if(Input_Device==INPUT_DEV_MOUSE)
	{
		if(PS2_MOUSE.status==MOUSE_NEW_DATA)
		{	
			PS2_Mouse_GetData();
			PS2_MOUSE.status=MOUSE_AKTIV;
			State.x=PS2_MOUSE.xPos;
			State.y=272-PS2_MOUSE.yPos;
			State.Pressed=0;
			if(PS2_MOUSE.LButton==BTN_PRESSED)
			{
				//if(State.Layer)
					//change_Layer=1;
				//State.Layer=0;
				State.Pressed|=1;
			}
			if(PS2_MOUSE.RButton==BTN_PRESSED)
			{
				//if(!State.Layer)
					//change_Layer=1;
				//State.Layer=1;
				State.Pressed|=2;
			}
			GUI_MOUSE_StoreState(&State);
			
			//current_widget=WM_Screen2hWin(State.x,State.y);
			//if(!WM_HasFocus(current_widget))
				//WM_SetFocus(current_widget);
			
			backlight_count=0;
			TIMx->CNT=0;
			TIM7->CNT=0;
			TIM13->CCR1=lcd_bright;
			if((TIM7->CR1&TIM_CR1_CEN)!=TIM_CR1_CEN)
				TIM7->CR1|=TIM_CR1_CEN;
		}
	}
	else
		Touch_GetData();
	
	temp=GUI_ALLOC_GetNumUsedBytes();
	if(GUI_UsedBytes!=temp)
	{	
		GUI_UsedBytes=temp;
		PROGBAR_SetValue(PROGBAR_MEM,GUI_UsedBytes);
		
	}
	
	TIM6->SR&=~TIM_SR_UIF; 							//Сбрасываем флаг UIF
	/* USER CODE END TIM6_DAC_IRQn 0 */
  //HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */
	
  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
* @brief This function handles TIM7 global interrupt.
*/
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
	
	backlight_count++;
	if(backlight_count==1)
		TIM13->CCR1=40;											// Уменьшаем ШИМ на подсветке	
  else if(backlight_count==2)			
	{
		backlight_count=0;
		TIM13->CCR1=0;											// Выключаем ШИМ на подсветке
		__HAL_TIM_DISABLE(&htim7);					// Выключаем таймер "время подсветки"	
	}
	/* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

void DMA2_Stream7_IRQHandler(void)
{
	//PROGBAR_Handle progbar;
	//uint8_t i;
	__HAL_DMA_CLEAR_FLAG(&hdma2,DMA_FLAG_TCIF3_7);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
					//uart_newmessage=0;
	/*progbar=WM_GetDialogItem(hWin2,ID_PROGBAR_1);
	__HAL_DMA_CLEAR_FLAG(&hdma2,DMA_FLAG_TCIF2_6);	
	
	
	if((size-transaction)<10000)
	{
		if((size-transaction)==0)
		{
			__HAL_DMA_DISABLE_IT(&hdma2,DMA_IT_TC);
			tx_message=1;
		}
		else
		{	
			for(i=0;i<9;i++)
				TxBuffer[i]=str2[i];							//		"get data"
		 USART1->DR=TxBuffer[0];
		 __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
		 DMA2_Stream2->M0AR=(uint32_t)(0xd0400000+transaction);	
		 DMA2_Stream2->NDTR=(uint32_t)(size-transaction);
		 __HAL_DMA_ENABLE(&hdma2); //DMA2_Stream2->CR |=DMA_SxCR_EN;
		 transaction+=(size-transaction);
		i=(transaction*100)/size;
		PROGBAR_SetValue(progbar,i);	
		}
	}
	else
	{
		
		for(i=0;i<9;i++)
			TxBuffer[i]=str2[i];							//	"get data"
		USART1->DR=TxBuffer[0];
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
		DMA2_Stream2->M0AR=(uint32_t)(0xd0400000+transaction);
		DMA2_Stream2->NDTR=10000;
		DMA2_Stream2->CR |=DMA_SxCR_EN;
		transaction+=10000;
		i=(transaction*100)/size;
		PROGBAR_SetValue(progbar,i);
		
	}*/
	
}

/**
* @brief This function handles LTDC global interrupt.
*/
void LTDC_IRQHandler(void)
{
  /* USER CODE BEGIN LTDC_IRQn 0 */

  /* USER CODE END LTDC_IRQn 0 */
  HAL_LTDC_IRQHandler(&hltdc);
  /* USER CODE BEGIN LTDC_IRQn 1 */

  /* USER CODE END LTDC_IRQn 1 */
}
/* USER CODE BEGIN 1 */
void TIM3_IRQHandler(void){
	
	//if(PS2_MOUSE.status==MOUSE_AKTIV)
	if(TIMx->SR&TIM_SR_UIF)
		{
			TIMx->SR&=~TIM_SR_TIF;
			if(PS2_MOUSE_VAR.mode==RX_MODE)
				PS2_Mouse_CheckScancode(scancode);
			else if (PS2_MOUSE_VAR.mode==TX_MODE)
				PS2_MOUSE_VAR.mode=RX_MODE;
			
			TIMx->SR&=~TIM_SR_UIF;
		}
	else if(TIMx->SR&TIM_SR_TIF)
		{
			PS2_Mouse_ISR();
			TIMx->SR&=~TIM_SR_TIF;
		}
}

void CAN2_RX0_IRQHandler(void){
	
	CAN_Receive_IRQHandler(0);		//FIFO0
	CAN_RXProcess0();
	/*Clear IT Pending Bit*/
	 
}


void CAN2_RX1_IRQHandler(void){
	 
	
	CAN_Receive_IRQHandler(1);		//FIFO1
	CAN_RXProcess1();
	

}

void CAN2_SCE_IRQHandler(void){
	
	GUI_DispChar('f');	

}
void EXTI2_IRQHandler (void){
	if(GPIOG->IDR & GPIO_IDR_IDR_2)
		hwin=2;
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
}
void EXTI15_10_IRQHandler (void){
	
	if(GPIOG->IDR & GPIO_IDR_IDR_10)
		hwin=10;
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
