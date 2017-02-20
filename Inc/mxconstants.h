/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
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

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
#define NONE (uint32_t)0x00000000
#define START_PWM (uint32_t)0x00000001
#define STOP_PWM	(uint32_t)0x00000002
#define START_ADC (uint32_t)0x00000003
#define STOP_ADC 	(uint32_t)0x00000004


#define constant 0x10

#define XPT2046_BUSY_Pin         GPIO_PIN_2
#define XPT2046_BUSY_GPIO_Port   GPIOE

#define XPT2046_IRQ_Pin          GPIO_PIN_8
#define XPT2046_IRQ_GPIO_Port    GPIOI
#define XPT2046_IRQ_MASK_IDR     GPIO_IDR_IDR_8


#define XPT2046_CS_Pin           GPIO_PIN_5
#define XPT2046_CS_GPIO_Port     GPIOD
#define XPT2046_CS_LOW			     GPIO_BSRR_BR_5
#define XPT2046_CS_HIGH			     GPIO_BSRR_BS_5

/* USER CODE BEGIN Private defines */
#define SET_MOUSE_RESOLUTION              0xE8
#define SET_MOUSE_SAMPLING_RATE           0xF3
#define ENABLE_MOUSE_TRANSMISSION         0xF4
#define SET_MOUSE_SCALING_1TO1            0xE6
#define READ_MOUSE_STATUS                 0xE9
#define GET_DEVICE_ID                     0xF2
#define MOUSE_RESET                       0xFF
/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
