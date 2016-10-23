/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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

/* USER CODE BEGIN Includes */
#include "GUI.h"
#include "DIALOG.h"
#include "PS2_Mouse.h"
#include "CAN.h"
#define SDRAM_BASE (uint32_t)0xD0000000  // swap SDRAM_Bank2 PCCARD_Bank  0xD0000000->0x90000000  
#define SDRAM_SIZE (uint32_t)0x800000
#define REFRESH_COUNT           ((uint32_t)1386)   /* SDRAM refresh counter */
#define SDRAM_TIMEOUT           ((uint32_t)0xFFFF)

//#define ID_ICON_date   (GUI_ID_USER + 0x05)

//#define ID_ICON_exit   (GUI_ID_USER + 0x07)
//#define ID_ICON_photo   (GUI_ID_USER + 0x08)

//#define ID_ICON_usb   (GUI_ID_USER + 0x0A)
//#define ID_ICON_alarmA   (GUI_ID_USER + 0x0B)
//#define ID_ICON_alarmB   (GUI_ID_USER + 0x0C)
//#define ID_ICON_alarmD   (GUI_ID_USER + 0x0D)

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

DMA2D_HandleTypeDef hdma2d;

LTDC_HandleTypeDef hltdc;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi5;

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim13;

SDRAM_HandleTypeDef hsdram2;


RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
RTC_AlarmTypeDef sAlarmA,sAlarmB;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

uint8_t Input_Device; 
extern PS2_MOUSE_t 		PS2_MOUSE;
 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_DMA2D_Init(void);
static void MX_FMC_Init(void);
static void MX_LTDC_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI5_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM13_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void SDRAM_Initialization_sequence(uint32_t RefreshCount);
extern void Touch_SendCMD(uint8_t Data);
extern uint16_t Touch_GetResult(void);
extern void Touch_calibration(void);
extern CANRX_TypeDef CAN_Data_RX1;

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)

{
  /* USER CODE BEGIN 1 */
		
	/* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
   /* USER CODE BEGIN 2 */
	SDRAM_Initialization_sequence(REFRESH_COUNT);	
	 /* USER CODE END 2 */
	MX_LTDC_Init();
  MX_RTC_Init();
  MX_SPI2_Init();
  MX_SPI5_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
	MX_TIM13_Init();
	 /* USER CODE BEGIN 3 */
	__HAL_SPI_ENABLE(&hspi2);
	
	DBGMCU->APB1FZ|=DBGMCU_APB1_FZ_DBG_TIM4_STOP;
	
	PS2_Mouse_Init();
	PS2_Mouse_Start();
	
	if(PS2_MOUSE.status==MOUSE_AKTIV)
	{
		Input_Device=INPUT_DEV_MOUSE;
		htim6.Instance = TIM6;
		htim6.Init.Prescaler = 44999;
		htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim6.Init.Period = 20;
		HAL_TIM_Base_Init(&htim6);
		XPT2046_CS_GPIO_Port->BSRR=GPIO_BSRR_BS_5;
	}
	else
	{
		Input_Device=INPUT_DEV_TOUCH;
		PS2_Mouse_DeInit();
		Touch_SendCMD(0xD0);
		Touch_GetResult();
		XPT2046_CS_GPIO_Port->BSRR=GPIO_BSRR_BS_5;
	}
		bxCAN_Init();
	
	/*__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
	__HAL_RTC_WAKEUPTIMER_DISABLE(&hrtc);
	while(__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_WUTWF) == RESET) {}
	__HAL_RTC_WAKEUPTIMER_ENABLE(&hrtc);	
	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);*/
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	
		
		
		GUI_Init();
		//LCD_SetSizeEx(0,200,250);
		//LCD_SetSizeEx(1,200,250);
		//LCD_SetVSizeEx(0, 200, 250);
		//LCD_SetVSizeEx(1, 200, 250);
		
		//GUI_SetLayerPosEx(0,0,0);
		//GUI_SetLayerPosEx(1,0,0);
		//LCD_SetChromaModeEx(0, ENABLE);
		//LCD_SetChromaEx(0,GUI_GREEN,GUI_GREEN);
		
		//LCD_SetChromaModeEx(1, ENABLE);
		//LCD_SetChromaEx(1,GUI_GREEN,GUI_GREEN);
		//GUI_SelectLayer(0);
		//Touch_calibration();
		//GUI_DrawBitmap(&bmDSC_0123, 0, 0);
		//LCD_SetAlphaEx(1, 0);
		//GUI_SetBkColor(GUI_LIGHTBLUE);
		//GUI_Clear();

	//__HAL_LTDC_LAYER_DISABLE(&hltdc, 1);
	// __HAL_LTDC_RELOAD_CONFIG(&hltdc); 
	//hltdc.Instance->SRCR = LTDC_SRCR_IMR;
		
		TIM7->DIER |= TIM_DIER_UIE; 				//разрешаем прерывание от таймера TIM7 "время подсветки"
		TIM7->EGR = TIM_EGR_UG;							//генерируем "update event". ARR и PSC грузятся из предварительного в теневой регистр. 
		TIM7->SR&=~TIM_SR_UIF; 							//Сбрасываем флаг UIF
		NVIC_ClearPendingIRQ(TIM7_IRQn);
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
		__HAL_TIM_ENABLE(&htim7);						// Запускаем таймер "время подсветки"	
		
		TIM6->DIER |= TIM_DIER_UIE; 				//разрешаем прерывание от таймера обновления структуры GUI_PID_STATE
		TIM6->EGR = TIM_EGR_UG;							//генерируем "update event". ARR и PSC грузятся из предварительного в теневой регистр. 
		TIM6->SR&=~TIM_SR_UIF; 							//Сбрасываем флаг UIF
		NVIC_ClearPendingIRQ(TIM6_DAC_IRQn);
		__HAL_TIM_ENABLE(&htim6);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);	
		
		HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
			
		MainTask();
			
		
	/* USER CODE END 3 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	 while (1)
  {
			
	}
	/* USER CODE END WHILE */
  
	/* USER CODE BEGIN 4 */
	
	/* USER CODE END 4 */
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  HAL_PWREx_ActivateOverDrive();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 288;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_16;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* CRC init function */
void MX_CRC_Init(void)
{

  hcrc.Instance = CRC;
  HAL_CRC_Init(&hcrc);

}

/* DMA2D init function */
void MX_DMA2D_Init(void)
{

  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_R2M;
  hdma2d.Init.ColorMode = DMA2D_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  HAL_DMA2D_Init(&hdma2d);

}

/* LTDC init function */
void MX_LTDC_Init(void)
{

  LTDC_LayerCfgTypeDef pLayerCfg;
  LTDC_LayerCfgTypeDef pLayerCfg1;

  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 40;
  hltdc.Init.VerticalSync = 9;
  hltdc.Init.AccumulatedHBP = 42;
  hltdc.Init.AccumulatedVBP = 11;
  hltdc.Init.AccumulatedActiveW = 522;
  hltdc.Init.AccumulatedActiveH = 283;
  hltdc.Init.TotalWidth = 524;
  hltdc.Init.TotalHeigh = 285;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  HAL_LTDC_Init(&hltdc);

  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0xd0000000;
  pLayerCfg.ImageWidth = 480;
  pLayerCfg.ImageHeight = 272;
  pLayerCfg.Backcolor.Blue = 150;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 100;
  HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0);

  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 480;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 272;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg1.Alpha = 255;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg1.FBStartAdress = 0xd00ff000;
  pLayerCfg1.ImageWidth = 480;
  pLayerCfg1.ImageHeight = 272;
  pLayerCfg1.Backcolor.Blue = 150;
  pLayerCfg1.Backcolor.Green = 100;
  pLayerCfg1.Backcolor.Red = 0;
  HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1);

}

/* RTC init function */
void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
  RTC_AlarmTypeDef sAlarm;

    /**Initialize RTC and set the Time and Date 
    */
 //
		hrtc.Instance = RTC;
		hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
		hrtc.Init.AsynchPrediv = 127;
		hrtc.Init.SynchPrediv = 255;
		hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
		hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
		hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
		HAL_RTC_Init(&hrtc);
		HAL_RTCEx_EnableBypassShadow(&hrtc);	
		HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 59, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
	
 if((RTC->ISR&RTC_ISR_INITS)!=RTC_ISR_INITS)
	{	
		sTime.Hours = 12;
		sTime.Minutes = 05;
		sTime.Seconds = 0;
		sTime.TimeFormat = RTC_HOURFORMAT12_AM;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);

		sDate.WeekDay = RTC_WEEKDAY_SATURDAY;
		sDate.Month = RTC_MONTH_FEBRUARY;
		sDate.Date = 27;
		sDate.Year = 16;
		HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BIN);
/**Enable the Alarm A 
    */
  sAlarm.AlarmTime.Hours = 12;
  sAlarm.AlarmTime.Minutes = 25;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, FORMAT_BIN);

    /**Enable the Alarm B 
    */
  sAlarm.AlarmDateWeekDay =1;
  sAlarm.Alarm = RTC_ALARM_B;
  HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, FORMAT_BIN);
	}
    
  

}

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLED;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  hspi2.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi2);

}

/* SPI5 init function */
void MX_SPI5_Init(void)
{

  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLED;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  hspi5.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi5);

}

/* TIM6 init function */
void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 44999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 200;
  HAL_TIM_Base_Init(&htim6);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);

}

/* TIM7 init function */
void MX_TIM7_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 44999;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 60000;
  HAL_TIM_Base_Init(&htim7);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig);

}
/* TIM14 init function */
void MX_TIM13_Init(void)
{
	TIM_OC_InitTypeDef  sConfig;
	
  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 4499;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 200;
	HAL_TIM_PWM_Init(&htim13);
	
	sConfig.OCMode=TIM_OCMODE_PWM2;
	sConfig.OCFastMode=TIM_OCFAST_DISABLE;
	sConfig.OCPolarity=TIM_OCPOLARITY_LOW;
	sConfig.Pulse=0;
	HAL_TIM_PWM_ConfigChannel(&htim13, &sConfig, TIM_CHANNEL_1);
	
	
}
/* FMC initialization function */
void MX_FMC_Init(void)
{
  FMC_SDRAM_TimingTypeDef SdramTiming;

  /** Perform the SDRAM2 memory initialization sequence
  */
  hsdram2.Instance = FMC_SDRAM_DEVICE;
  /* hsdram2.Init */
  hsdram2.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram2.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram2.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram2.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram2.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram2.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram2.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram2.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram2.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram2.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_2;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 2;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  HAL_SDRAM_Init(&hsdram2, &SdramTiming);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOE_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOI_CLK_ENABLE();
  __GPIOF_CLK_ENABLE();
  __GPIOH_CLK_ENABLE();
  __GPIOG_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : XPT2046_BUSY_Pin XPT2046_IRQ_Pin */
  GPIO_InitStruct.Pin = XPT2046_BUSY_Pin|XPT2046_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : XPT2046_CS_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	/*Configure GPIO pin : Joystick */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void SDRAM_Initialization_sequence(uint32_t RefreshCount)
{
  
	#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
	#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
	#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
	#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
	#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
	#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
	#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
	#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
	#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
	#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
	#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)
	
	FMC_SDRAM_CommandTypeDef Command;
	
		
	__IO uint32_t tmpmrd =0;
  
  /* Step 1:  Configure a clock configuration enable command */
  Command.CommandMode             = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber       = 1;
  Command.ModeRegisterDefinition  = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram2, &Command, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */ 
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);

  /* Step 3: Configure a PALL (precharge all) command */ 
  Command.CommandMode             = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber       = 1;
  Command.ModeRegisterDefinition  = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram2, &Command, SDRAM_TIMEOUT);  
  
  /* Step 4: Configure an Auto Refresh command */ 
  Command.CommandMode             = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber       = 4;
  Command.ModeRegisterDefinition  = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram2, &Command, SDRAM_TIMEOUT);
  
  /* Step 5: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_3           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  
  Command.CommandMode             = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber       = 1;
  Command.ModeRegisterDefinition  = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram2, &Command, SDRAM_TIMEOUT);
  
  /* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(&hsdram2, RefreshCount); 

// Clear SDRAM 
	while(FMC_Bank5_6->SDSR&FMC_SDSR_BUSY);
	for(tmpmrd=SDRAM_BASE;tmpmrd<(SDRAM_BASE+SDRAM_SIZE);tmpmrd+=4)
	*(uint32_t*)tmpmrd=0x0;
}

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
