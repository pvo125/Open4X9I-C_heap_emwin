#ifndef _PS2_Mouse_H_
#define _PS2_Mouse_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"


#define 	CLOCK_PIN								7				
#if (CLOCK_PIN>7)
	#define AFR	 AFR[1]	
#else
	#define AFR  AFR[0]	
#endif

#define GPIO_AFIO									(uint32_t)(0x20000000)
#define __TIMx_CLK_ENABLE()				__TIM3_CLK_ENABLE()
#define __TIMx_CLK_DISABLE()			__TIM3_CLK_DISABLE()
#define TIMx_IRQn									TIM3_IRQn
#define TIMx											TIM3
/**************************************************************************/
#define DATA_BSRR_BR							GPIO_BSRR_BR_1
#define DATA_BSRR_BS							GPIO_BSRR_BS_1
#define DATA_IDR_IDRx							GPIO_IDR_IDR_1

#define DATA_ODR_ODRx							GPIO_ODR_ODR_1
#define DATA_MODER_MODERx					GPIO_MODER_MODER1_0
#define DATA_OTYPER_OTx						GPIO_OTYPER_OT_1
#define DATA_OSPEEDER_OSPEEDRx		GPIO_OSPEEDER_OSPEEDR1_0	
#define DATA_PUPDR_PUPDRx					GPIO_PUPDR_PUPDR1_0
/************************************************************************/
#define CLOCK_BSRR_BR							GPIO_BSRR_BR_7
#define CLOCK_BSRR_BS							GPIO_BSRR_BS_7

#define CLOCK_ODR_ODRx						GPIO_ODR_ODR_7
#define CLOCK_MODER_MODERx				GPIO_MODER_MODER7_0
#define CLOCK_MODER_MODERx_1			GPIO_MODER_MODER7_1
#define CLOCK_OTYPER_OTx					GPIO_OTYPER_OT_7
#define CLOCK_OSPEEDER_OSPEEDRx		GPIO_OSPEEDER_OSPEEDR7_0	
#define CLOCK_PUPDR_PUPDRx				GPIO_PUPDR_PUPDR7_0
/**************************************************************************/
#define MOUSE_DATA_PORT						GPIOC
#define MOUSE_CLOCK_PORT					GPIOA

/***************************************************************************/


extern volatile uint8_t PS2_NewData;

#define INPUT_DEV_TOUCH  			 1
#define INPUT_DEV_MOUSE        0


#define  PS2_MOUSE_XPOS_MAX    480
#define  PS2_MOUSE_XPOS_START  240

#define  PS2_MOUSE_YPOS_MAX    272
#define  PS2_MOUSE_YPOS_START  136

#define TX_MODE									1
#define RX_MODE									0

#define  PS2_MOUSE_CMD_ID        				0xF2  // ID auslesen
#define  PS2_MOUSE_CMD_SAMPLE    				0xF3  // Samplerate einstellen
#define  PS2_MOUSE_CMD_ENABLE   				0xF4  // Mouse Enable
#define  PS2_MOUSE_CMD_ACK       				0xFA  // ACK-Antwort
#define  PS2_MOUSE_CMD_RESET     				0xFF  // Reset
#define  PS2_MOUSE_CMD_SET_RESOLUTION   0xE8  // SET_RESOLUTION
#define  PS2_MOUSE_CMD_STREAM_MODE	    0xEA  // SET STREAM MODE

typedef enum {
  MOUSE_TYP_UNKNOWN =0, //  Mousetyp unbekannt
  MOUSE_TYP_STANDARD,   //  Standard-Mouse (ohne Mausrad)
  MOUSE_TYP_INTELLI     //  Intelli-Mouse (mit Mausrad)
}PS2_MOUSE_TYP_t;

typedef enum {
  BTN_RELEASED =0,      // Taste losgelassen
  BTN_PRESSED           // Taste bet嵩gt
}PS2_MOUSE_BTN;

typedef enum {
  MOUSE_INAKTIV = 0,    // Mouse abgeschaltet
  MOUSE_AKTIV,          // Mouse eingeschaltet
  MOUSE_NEW_DATA        // Neue Daten vorhanden
}PS2_MOUSE_STATUS_t;



typedef struct {
  uint8_t mode;
	uint8_t receive_code;   // empfangene Daten
  uint8_t send_code;      // gesendete Daten
  uint8_t buff[4];         // Datenpuffer
}PS2_MOUSE_VAR_t;



typedef struct{
	PS2_MOUSE_BTN LButton;
	PS2_MOUSE_BTN	RButton;
	uint16_t	xPos;
	uint16_t	yPos;
	PS2_MOUSE_TYP_t type;
	PS2_MOUSE_STATUS_t status;
}PS2_MOUSE_t;



void PS2_Mouse_Init(void);
void PS2_Mouse_DeInit(void);
ErrorStatus PS2_Mouse_Start(void);
ErrorStatus PS2_Mouse_Timeout_TX ( volatile uint32_t nCount );
ErrorStatus PS2_Mouse_Timeout_RX ( volatile uint32_t nCount );
ErrorStatus PS2_Mouse_Tx(uint8_t cmd);
void PS2_Mouse_GetData(void);
void PS2_Mouse_CheckScancode(uint8_t scan);
void PS2_Mouse_ISR(void);

/****************************************************/
#endif
