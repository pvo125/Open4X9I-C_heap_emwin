/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.22                          *
*        Compiled Jul  4 2013, 15:16:01                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END
#include "stm32f4xx_hal.h"
#include "DIALOG.h"
#include "MESSAGEBOX.h"
#include "GUI.h"
#include "LCDConf.h"
#include "PS2_Mouse.h" 
#include "CAN.h"
#include <string.h>
/*********************************************************************
*       									Defines
**********************************************************************/
#define ID_WINDOW_0     (GUI_ID_USER + 0x01)
#define ID_WINDOW_1     (GUI_ID_USER + 0x02)
#define ID_WINDOW_2     (GUI_ID_USER + 0x03)

#define ID_ICON_alarm   (GUI_ID_USER + 0x04)
#define ID_ICON_settings   (GUI_ID_USER + 0x05)
#define ID_ICON_paint   (GUI_ID_USER + 0x06)

#define ID_ICON_date   (GUI_ID_USER + 0x07)
#define ID_ICON_net   (GUI_ID_USER + 0x08)
#define ID_ICON_usb   (GUI_ID_USER + 0x09)

#define ID_ICON_alarma   (GUI_ID_USER + 0x0A)
#define ID_ICON_alarmb   (GUI_ID_USER + 0x0B)
#define ID_SLIDER_0     (GUI_ID_USER + 0xC)
#define ID_PROGBAR     	(GUI_ID_USER + 0xD)
#define ID_BUTTON_NEXT	(GUI_ID_USER + 0xE)
#define ID_BUTTON_PREV	(GUI_ID_USER + 0xF)

#define ID_PROGBAR_1     	(GUI_ID_USER + 0x3C)
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;

extern RTC_AlarmTypeDef sAlarmA,sAlarmB;

extern GUI_PID_STATE State;
extern LTDC_HandleTypeDef hltdc;
extern RTC_HandleTypeDef hrtc;

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef  hdma2;

extern GUI_CONST_STORAGE GUI_BITMAP bmalarm;
extern GUI_CONST_STORAGE GUI_BITMAP bmpaint;
extern GUI_CONST_STORAGE GUI_BITMAP bmsettings;

extern GUI_CONST_STORAGE GUI_BITMAP bmdate;
extern GUI_CONST_STORAGE GUI_BITMAP bmcan;
extern GUI_CONST_STORAGE GUI_BITMAP bmusb;
extern GUI_CONST_STORAGE GUI_BITMAP bmxp;
extern GUI_CONST_STORAGE GUI_BITMAP bmalarm_a;
extern GUI_CONST_STORAGE GUI_BITMAP bmalarm_a_d;
extern GUI_CONST_STORAGE GUI_BITMAP bmalarm_b;
extern GUI_CONST_STORAGE GUI_BITMAP bmalarm_b_d;
extern GUI_CONST_STORAGE GUI_BITMAP bmupdate;

#ifdef CODEFLASH
	const GUI_BITMAP *Icons0[]={&bmalarm,&bmsettings,&bmpaint};
	const GUI_BITMAP *Icons1[]={&bmdate,&bmcan,&bmusb};
	const GUI_BITMAP *Icons2[]={&bmalarm_a,&bmalarm_b,&bmalarm_a_d,&bmalarm_b_d};
#endif	
extern void Touch_calibration(void);
extern LCD_LayerPropTypedef          layer_prop[GUI_NUM_LAYERS];


extern PS2_MOUSE_VAR_t PS2_MOUSE_VAR;
extern uint8_t Input_Device;
extern uint8_t backlight_count;
extern WM_HWIN hWin_CANNetExplore;
extern WM_HWIN CANNetExplore(void);
WM_HWIN Window_0(void);
WM_HWIN Window_1(void);
WM_HWIN Layer_1(void);
PROGBAR_Handle PROGBAR_MEM;
WM_HWIN _Paint(void); 

WM_HWIN hWin_paint;

GUI_MEMDEV_Handle hMemShadow,hMemShadow1;
uint8_t Layer;

char str1[]="get size";
char str2[]="get data";
char str3[]="crc ok!";
char str4[]="crc error!";
uint8_t RxBuffer[16];
uint8_t TxBuffer[16];

volatile uint8_t uart_newmessage;
volatile uint8_t uart_data_message=0;
volatile uint8_t uart_get_size=0;
volatile uint8_t uart_get_data=0;
volatile uint8_t uart_end_transaction=0;

//volatile uint8_t tx_message=0;
//volatile uint8_t rx_message=0;

volatile int size=0,dec=1;
volatile int transaction;

extern volatile uint8_t message_flag;
extern volatile uint8_t new_firmware;
extern const char  *netname_array[];

WM_HWIN hWin0,hWin1,hWin2;
extern WM_HWIN CreateTIME_DATE(void);
extern WM_HWIN CreateALARM(void);

void UART_Terminal_DMATran(char *p);
uint32_t crc32_check(const uint8_t *buff,uint32_t count);

/*********************************************************************
*       _cbBK
*/
/*static void _cbBK(WM_MESSAGE * pMsg) {
  int     NCode;
  int     Id;
	
  switch (pMsg->MsgId) {
	case WM_PAINT:
			GUI_Clear();
	break;	
	 default:
    WM_DefaultProc(pMsg);
    break;
  }
}*/
/*********************************************************************
*       								_cbPaint_0
**********************************************************************/
static void _cbPaint_0(WM_MESSAGE * pMsg) {
 
	switch (pMsg->MsgId) {
	case WM_PRE_PAINT:
		GUI_MULTIBUF_BeginEx(0);
	break;
	case WM_PAINT:
		GUI_SetBkColor(0xD0D0D0);
		GUI_Clear();
	break;
	case WM_POST_PAINT:
	GUI_MULTIBUF_EndEx(0);
	break;
	case WM_TOUCH:
	break;	
	default:
    WM_DefaultProc(pMsg);
    break;
  }
}
	
/*********************************************************************
*       								_cbPaint_1
**********************************************************************/
static void _cbPaint_1(WM_MESSAGE * pMsg) {
 
	switch (pMsg->MsgId) {
	case WM_PRE_PAINT:
		GUI_MULTIBUF_BeginEx(1);
	break;
	case WM_PAINT:
		
		GUI_SetColor(GUI_DARKBLUE);
		GUI_SetPenSize(10);
		GUI_DrawPoint(State.x,State.y);
	break;
	case WM_POST_PAINT:
	GUI_MULTIBUF_EndEx(1);
	break;
	case WM_TOUCH:
		WM_Paint(hWin_paint);
	break;	
	default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*       									_Paint
**********************************************************************/
WM_HWIN _Paint(void){
			
	WM_CreateWindowAsChild(0,0,480,272,WM_GetDesktopWindowEx(0),WM_CF_SHOW, _cbPaint_0,0);	
	hWin_paint=WM_CreateWindowAsChild(0,0,480,272,WM_GetDesktopWindowEx(1),WM_CF_SHOW, _cbPaint_1,0);
	Layer=1;
	return hWin_paint;
}
	
/*********************************************************************
*       									_cbWin_0
**********************************************************************/
static void _cbWin_0(WM_MESSAGE * pMsg) {
  int     NCode;
  int     Id;
	

  switch (pMsg->MsgId) {
	case WM_PID_STATE_CHANGED:
		Layer=1;
	break;
	case WM_PRE_PAINT:
		GUI_MULTIBUF_BeginEx(0);
	break;
	case WM_PAINT:
		#ifdef CODEFLASH
			GUI_DrawBitmap(&bmxp,0,0);
		#else	
			GUI_SetBkColor(GUI_LIGHTBLUE);
			GUI_Clear();
		#endif
	break;
	case WM_POST_PAINT:
		GUI_MULTIBUF_EndEx(0);
	break;
	case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_ICON_alarm: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
			if(!(WM_GetDialogItem(hWin0,(GUI_ID_USER + 0x0F))))
				#ifdef CODEFLASH
					CreateALARM();
				#endif
			break;
			}
    break;
    case ID_ICON_paint: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
			/*WM_HideWindow(hWin0);
			WM_HideWindow(hWin2);
			GUI_SelectLayer(1);
			GUI_SetBkColor(GUI_TRANSPARENT);
			GUI_Clear();
			_Paint();*/
			break;
      }
    break;
    case ID_ICON_settings: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hWin0);
				WM_HideWindow(hWin2);
			//#ifdef CODEFLASH	
					Touch_calibration();	
				//#endif
			break;
      }
    break;
    }
  break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*       									_cbWin_1
**********************************************************************/
static void _cbWin_1(WM_MESSAGE * pMsg) {
  int     NCode;
  int     Id;
  switch (pMsg->MsgId) {
	case WM_PID_STATE_CHANGED:
		Layer=1;
		
	break;
	case WM_PRE_PAINT:
		GUI_MULTIBUF_BeginEx(0);
	break;
	case WM_PAINT:
		#ifdef CODEFLASH
			GUI_DrawBitmap(&bmxp,0,0);
		#else	
			GUI_SetBkColor(GUI_LIGHTBLUE);
			GUI_Clear();
		#endif
	break;
	case WM_POST_PAINT:
	GUI_MULTIBUF_EndEx(0);
	break;
	case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_ICON_date: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
			if(!(WM_GetDialogItem(hWin1,(GUI_ID_USER + 0x0F))))
				#ifdef CODEFLASH 
					CreateTIME_DATE();
				#endif
			break;
			}
    break;
    case ID_ICON_net: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
				if(hWin_CANNetExplore==0)
					CANNetExplore();
			break;
      }
    break;
    case ID_ICON_usb: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
			break;
      }
    break;
    }
  break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}



/*********************************************************************
*      										_cbLayer_1
**********************************************************************/
static void _cbLayer_1(WM_MESSAGE * pMsg) {
  
	int     NCode;
  int     Id;
	WM_HWIN hWin;
	ICONVIEW_Handle hIcon;
	int i;
	
	
  switch (pMsg->MsgId) {
		case WM_TOUCH://WM_PID_STATE_CHANGED:
		State.Layer=0;
		GUI_PID_StoreState(&State);
	break;
	case WM_PRE_PAINT:
		GUI_MULTIBUF_BeginEx(1);
	break;
	case WM_PAINT:
		
		GUI_SetBkColor(GUI_TRANSPARENT);
		GUI_Clear();
		#ifdef CODEFLASH
			if(new_firmware)
				GUI_DrawBitmap(&bmupdate,255,0);
						
			GUI_MEMDEV_CopyToLCDAt(hMemShadow,15,0);
			GUI_MEMDEV_CopyToLCD(hMemShadow);	
			
			GUI_SetTextMode(GUI_TM_TRANS);
			GUI_SetColor(GUI_YELLOW);
			GUI_SetFont(&GUI_Font8x16);
			
			GUI_DispDecAt(sDate.Date,28,8,2);
			GUI_DispChar('.');
			GUI_DispDec(sDate.Month,2);	
			GUI_DispChar('.');
			GUI_DispDec(sDate.Year,2);
		
			GUI_DispDecAt(sTime.Hours,412,8,2);
			GUI_DispChar(':');
			GUI_DispDec(sTime.Minutes,2);
		#endif
		
		
	break;
	case WM_POST_PAINT:
		GUI_MULTIBUF_EndEx(1);
	
	break;
	case WM_USER:
		WM_Paint(hWin2);
	break;	
	case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
			case ID_SLIDER_0: // Notifications sent by 'Slider'
      switch(NCode) {
      case WM_NOTIFICATION_VALUE_CHANGED:
				hWin=WM_GetDialogItem(hWin2, ID_SLIDER_0);
				i=SLIDER_GetValue(hWin);
				if(i==1)
					{
						WM_HideWindow(hWin1);
						WM_ShowWindow(hWin0);
					}
				else if(i==2)
					{	
					WM_HideWindow(hWin0);
					WM_ShowWindow(hWin1);
					}
			break;
			case WM_NOTIFICATION_RELEASED:
			break;
			}
    break;
		case ID_ICON_alarma: // Notifications sent by 'ICON_alarma'
      switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				hIcon=WM_GetDialogItem(hWin2,ID_ICON_alarma);
				WM_DeleteWindow(hIcon);
				hIcon=ICONVIEW_CreateEx(300,2,34,34,hWin2,WM_CF_SHOW|WM_CF_HASTRANS,0,(GUI_ID_USER + 0x0A),24,24);
				ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_DARKBLUE|0x80000000);
				if((RTC->CR&RTC_CR_ALRAE)==RTC_CR_ALRAE)
							{
								HAL_RTC_DeactivateAlarm(&hrtc,  RTC_ALARM_A);
									#ifdef CODEFLASH
										ICONVIEW_AddBitmapItem(hIcon,Icons2[2],"");
									#endif
								
							}
							else
							{
								HAL_RTC_SetAlarm_IT(&hrtc,&sAlarmA,RTC_FORMAT_BIN);
									#ifdef CODEFLASH
										ICONVIEW_AddBitmapItem(hIcon,Icons2[0],"");
									#endif
							}
			break;	
			case WM_NOTIFICATION_RELEASED:
				hWin=WM_GetDialogItem(hWin2,ID_SLIDER_0);
				WM_SetFocus(hWin);
			break;
			}
    break;
		case ID_ICON_alarmb: // Notifications sent by 'ICON_alarma'
      switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				hIcon=WM_GetDialogItem(hWin2,ID_ICON_alarmb);
				WM_DeleteWindow(hIcon);
				hIcon=ICONVIEW_CreateEx(336,2,34,34,hWin2,WM_CF_SHOW|WM_CF_HASTRANS,0,(GUI_ID_USER + 0x0B),24,24);
				ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_DARKBLUE|0x80000000);	
				if((RTC->CR&RTC_CR_ALRBE)==RTC_CR_ALRBE)
					{
						HAL_RTC_DeactivateAlarm(&hrtc,  RTC_ALARM_B);
							#ifdef CODEFLASH
								ICONVIEW_AddBitmapItem(hIcon,Icons2[3],"");
							#endif
					}
				else
					{
						HAL_RTC_SetAlarm_IT(&hrtc,&sAlarmB,RTC_FORMAT_BIN);
							#ifdef CODEFLASH
								ICONVIEW_AddBitmapItem(hIcon,Icons2[1],"");
							#endif
					}
			break;	
			case WM_NOTIFICATION_RELEASED:
				hWin=WM_GetDialogItem(hWin2,ID_SLIDER_0);
				WM_SetFocus(hWin);
			break;
			}
    break;			
		}
  break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}
/*********************************************************************
*       										Window_0
**********************************************************************/
WM_HWIN Window_0(void) {
	
		uint8_t i;
		ICONVIEW_Handle hIcon;
		
		hWin0=WM_CreateWindowAsChild(0,0,480,272,WM_GetDesktopWindowEx(0),0, _cbWin_0,0);	
		for(i=0;i<3;i++)
		{
			hIcon=ICONVIEW_CreateEx(30,30+70*i,58,58,hWin0,WM_CF_SHOW|WM_CF_HASTRANS,0,(GUI_ID_USER + 0x04+i),48,48);
				#ifdef CODEFLASH
					ICONVIEW_AddBitmapItem(hIcon,Icons0[i],"");
				#endif
			ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_RED|0xC0000000);	
		}
		return hWin0;
}
/*********************************************************************
*       										Window_1
**********************************************************************/
WM_HWIN Window_1(void) {
	
		uint8_t i;
		ICONVIEW_Handle hIcon;
			
		hWin1=WM_CreateWindowAsChild(0,0,480,272,WM_GetDesktopWindowEx(0),0, _cbWin_1,0);	
		for(i=0;i<3;i++)
		{
			hIcon=ICONVIEW_CreateEx(30,30+70*i,58,58,hWin1,WM_CF_SHOW|WM_CF_HASTRANS,0,(GUI_ID_USER + 0x07+i),48,48);
				#ifdef CODEFLASH
					ICONVIEW_AddBitmapItem(hIcon,Icons1[i],"");
				#endif
			ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_RED|0xC0000000);
		}
		return hWin1;
}
/*********************************************************************
*      											Layer_1
**********************************************************************/
WM_HWIN Layer_1(void) {
	
		uint8_t i;
		ICONVIEW_Handle hIcon;	
		SLIDER_Handle hSlider;
		
		hWin2=WM_CreateWindowAsChild(0,0,480,272,WM_GetDesktopWindowEx(1),WM_CF_SHOW, _cbLayer_1,0);
		
		PROGBAR_MEM=PROGBAR_CreateEx(0,0,15,272,hWin2,WM_CF_SHOW|WM_CF_HASTRANS,PROGBAR_CF_VERTICAL,ID_PROGBAR);
		PROGBAR_SetMinMax(PROGBAR_MEM,0,50*1024);	
		
		for(i=0;i<2;i++)
		{
			hIcon=ICONVIEW_CreateEx(300+36*i,2,34,34,hWin2,WM_CF_SHOW|WM_CF_HASTRANS,0,(GUI_ID_USER + 0x0A+i),24,24);
				#ifdef CODEFLASH
					ICONVIEW_AddBitmapItem(hIcon,Icons2[i],"");
				#endif
			ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_DARKBLUE|0x80000000);
		}
		hSlider=SLIDER_CreateEx(190,205,120, 30,hWin2, WM_CF_SHOW|WM_CF_HASTRANS,0, ID_SLIDER_0);
		SLIDER_SetNumTicks(hSlider,2);
		SLIDER_SetRange(hSlider,1,2);
		return hWin2;
}
/**********************************************************************
*														MainTask
**********************************************************************/
void MainTask(void){
	PROGBAR_Handle progbar;
	uint32_t crc;
	uint8_t i,m;
	char temp;
	char *p;
	
	#ifdef CODEFLASH
		GUI_MEMDEV_Handle hmem1,hmem2,hmem_L,hmem_R,hmem_T,hmem_B,hmemH,hmemV;
	#endif
	
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkinClassic();
	FRAMEWIN_SetDefaultTextColor(0,GUI_WHITE);
	FRAMEWIN_SetDefaultTextColor(1,GUI_WHITE);
	
	EDIT_SetDefaultBkColor(EDIT_CI_ENABLED,GUI_BLACK);
	EDIT_SetDefaultFont(GUI_FONT_16B_ASCII);
		
	TEXT_SetDefaultFont(GUI_FONT_16_ASCII);
		
	//WM_EnableMemdev(1);
	WM_MULTIBUF_Enable(1);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetAlarm(&hrtc, &sAlarmA, RTC_ALARM_A,RTC_FORMAT_BIN);
	HAL_RTC_GetAlarm(&hrtc, &sAlarmB, RTC_ALARM_B,RTC_FORMAT_BIN);
	
	
	Window_0();
	Window_1();
		
	#ifdef CODEFLASH
		GUI_SelectLayer(1);
		
		hMemShadow=GUI_MEMDEV_CreateFixed(390,0,90,30,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUICC_8888);
		GUI_MEMDEV_Select(hMemShadow);
		GUI_SetBkColor(GUI_TRANSPARENT);
		GUI_Clear();
		GUI_SetColor(GUI_GRAY);
		GUI_FillRect(400,10,469,19);
		
		hmem1=GUI_MEMDEV_CreateFixed(0,0,20,90,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUICC_8888);
		GUI_MEMDEV_Select(hmem1);
		GUI_SetBkColor(GUI_TRANSPARENT);
		GUI_Clear();
		GUI_DrawGradientH(0,0,20,90,GUI_TRANSPARENT,GUI_GRAY);
		
		hmem2=GUI_MEMDEV_CreateFixed(0,0,20,30,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUICC_8888);
		GUI_MEMDEV_Select(hmem2);
		GUI_SetBkColor(GUI_TRANSPARENT);
		GUI_Clear();
		GUI_DrawGradientH(0,0,20,30,GUI_TRANSPARENT,GUI_GRAY);
		
		hmemH=GUI_MEMDEV_CreateFixed(0,0,10,90,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUICC_8888);
		GUI_MEMDEV_Select(hmemH);
		GUI_SetBkColor(GUI_TRANSPARENT);
		GUI_Clear();
		
		hmemV=GUI_MEMDEV_CreateFixed(0,0,10,30,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUICC_8888);
		GUI_MEMDEV_Select(hmemV);
		GUI_SetBkColor(GUI_TRANSPARENT);
		GUI_Clear();
		
		
		hmem_L=GUI_MEMDEV_CreateFixed(0,0,10,30,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUICC_8888);
		GUI_MEMDEV_Select(hmem_L);
		GUI_SetBkColor(GUI_TRANSPARENT);
		GUI_Clear();
		hmem_R=GUI_MEMDEV_CreateFixed(0,0,10,30,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUICC_8888);
		GUI_MEMDEV_Select(hmem_R);
		GUI_SetBkColor(GUI_TRANSPARENT);
		GUI_Clear();
		hmem_T=GUI_MEMDEV_CreateFixed(0,0,90,10,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUICC_8888);
		GUI_MEMDEV_Select(hmem_T);
		GUI_SetBkColor(GUI_TRANSPARENT);
		GUI_Clear();
		hmem_B=GUI_MEMDEV_CreateFixed(0,0,90,10,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUICC_8888);
		GUI_MEMDEV_Select(hmem_B);
		GUI_SetBkColor(GUI_TRANSPARENT);
		GUI_Clear();
		
			
		GUI_MEMDEV_Select(hmemH);
		GUI_MEMDEV_DrawPerspectiveX(hmem1, 0,0,90,70,10,10);
		GUI_MEMDEV_Select(hmemV);
		GUI_MEMDEV_DrawPerspectiveX(hmem2, 0,1,28,8,10,10);
				
		GUI_MEMDEV_Rotate(hmemH,hmem_T,40,-40, -90*1000,1000);
		GUI_MEMDEV_Rotate(hmemH,hmem_B,40,-40, 90*1000,1000);
		GUI_MEMDEV_Rotate(hmemV,hmem_R,0,0,180*1000,1000);
		
		GUI_MEMDEV_Select(hMemShadow);
		GUI_MEMDEV_WriteAt(hmem_T, 390, 0);
		GUI_MEMDEV_WriteAt(hmem_B, 390, 20);
		GUI_MEMDEV_WriteAt(hmemV, 390, 0);
		GUI_MEMDEV_WriteAt(hmem_R, 470, 0);
		
		GUI_MEMDEV_Select(0);
		
		GUI_MEMDEV_Delete(hmem1);
		GUI_MEMDEV_Delete(hmem2);
		GUI_MEMDEV_Delete(hmem_L);
		GUI_MEMDEV_Delete(hmem_R);
		GUI_MEMDEV_Delete(hmem_T);
		GUI_MEMDEV_Delete(hmem_B);
		GUI_MEMDEV_Delete(hmemH);
		GUI_MEMDEV_Delete(hmemV);
	#endif
	Layer_1();
	//WM_ShowWindow(hWin1);	
	WM_ShowWindow(hWin0);
	if(Input_Device==1)
	{GUI_CURSOR_Show();
		Layer=1;
		State.y=136;
		State.x=240;
		//GUI_MOUSE_StoreState(&State);
		//GUI_CURSOR_SetPosition(State.x,State.y);
	}
	for(i=20;i<70;i++)
		{
			TIM13->CCR1=i;
			GUI_Delay(50);
		}
		
while(1)	
{			
			State.Layer=Layer;
		//GUI_TOUCH_StoreStateEx(&State);	
		//GUI_MOUSE_StoreState(&State);
		//if(Input_Device==INPUT_DEV_MOUSE)
		//{
		//GUI_MOUSE_GetState(&State);
		//GUI_CURSOR_SetPosition(State.x,State.y);
		//}
		//else
		//	GUI_TOUCH_StoreStateEx(&State);
			if(uart_newmessage)
			{
				if((strcmp("hello st\r",(const char*)RxBuffer))==0)
				{
					*(uint64_t*)(RxBuffer+0)=(uint64_t)0;
					*(uint64_t*)(RxBuffer+8)=(uint64_t)0;	
					
					// перенастроим DMA для приема след. сообщения
					DMA2_Stream2->CR &=~DMA_SxCR_EN;
					DMA2_Stream2->NDTR=16;
					DMA2->LIFCR |=DMA_LIFCR_CTCIF2|DMA_LIFCR_CHTIF2;
					DMA2_Stream2->CR |=DMA_SxCR_EN;
					uart_newmessage=0;
					
					__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
					UART_Terminal_DMATran("get size");
					uart_data_message=1;
				}
				else
				{
					*(uint64_t*)(RxBuffer+0)=(uint64_t)0;
					*(uint64_t*)(RxBuffer+8)=(uint64_t)0;
									
					DMA2_Stream2->CR &=~DMA_SxCR_EN;
					DMA2_Stream2->NDTR=20;
					DMA2->LIFCR |=DMA_LIFCR_CTCIF2|DMA_LIFCR_CHTIF2;
					DMA2_Stream2->CR |=DMA_SxCR_EN;
					uart_newmessage=0;
				}
			}
			if(uart_get_size)
			{
				size=0;
				m=strlen((char*)RxBuffer)-1;
				p=(char*)(RxBuffer+m-1);
				for(i=0;i<m;i++)
				{
					temp=*(char*)(p-i);
					size+=(temp&0x0f)*dec;
					dec*=10;
				}
				__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
				UART_Terminal_DMATran("get data");
								
				backlight_count=0;
				TIM7->CNT=0;
				TIM13->CCR1=70;
						
				DMA2_Stream2->CR &=~DMA_SxCR_EN;
				DMA2_Stream2->NDTR=20000;
				DMA2_Stream2->M0AR=0xD0400000;
				DMA2->LIFCR |=DMA_LIFCR_CTCIF2|DMA_LIFCR_CHTIF2;
				DMA2_Stream2->CR |=DMA_SxCR_EN;
								
				uart_data_message=1;
				transaction=0;		
				uart_get_size=0;
			}
			if(uart_get_data)
			{
				uart_data_message=1;
				uart_get_data=0;
								
				DMA2_Stream2->CR &=~DMA_SxCR_EN;
				transaction+=(19999-DMA2_Stream2->NDTR);
				DMA2_Stream2->M0AR+=(19999-DMA2_Stream2->NDTR);
				DMA2->LIFCR |=DMA_LIFCR_CTCIF2|DMA_LIFCR_CHTIF2;
				DMA2_Stream2->CR |=DMA_SxCR_EN;
				if((size-transaction)>0)
				{
					__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
					UART_Terminal_DMATran("get data");
				}
				else
				{
					uart_end_transaction=1;
					uart_get_data=0;
				}
			}
			if(uart_end_transaction)
			{
				crc=crc32_check((const uint8_t *)0xd0400000,(size-4));
				uart_data_message=0;
				uart_end_transaction=0;
				// перенастроим DMA для приема след. сообщения
				DMA2_Stream2->CR &=~DMA_SxCR_EN;
				DMA2_Stream2->M0AR=(uint32_t)RxBuffer;
				DMA2_Stream2->NDTR=16;
				DMA2->LIFCR |=DMA_LIFCR_CTCIF2|DMA_LIFCR_CHTIF2;
				DMA2_Stream2->CR |=DMA_SxCR_EN;
				if(crc==*(uint32_t*)(0xd0400000+size-4))
				{
					__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
					UART_Terminal_DMATran("crc ok!");
					
					new_firmware=0;
					while(strcmp((const char*)netname_array[new_firmware],(const char*)0xD0400020))
						new_firmware++;	
					
					progbar=PROGBAR_CreateEx(246,32,50,5,hWin2,WM_CF_HASTRANS,PROGBAR_CF_HORIZONTAL,ID_PROGBAR_1);
					PROGBAR_SetText(progbar,"");	
					PROGBAR_SetMinMax(progbar,0,100);
					
					WM_SendMessageNoPara(hWin2,WM_USER);// сообщение окну hWin2 верхнего слоя layer 1 перерисовыаться и поверх символ update
				}
				else
				{
					__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
					UART_Terminal_DMATran("crc error!");
				}
				dec=1;
				
			}
			
		
			if(message_flag)
				{
					TEXT_SetDefaultTextColor(GUI_BLACK);
					GUI_SelectLayer(0);
					if(message_flag==1)
					{
						//__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
						//UART_Terminal_DMATran("Download complete!\r\n");
						WM_DeleteWindow(progbar);
						
						MESSAGEBOX_Create("Download complete!","MESSAGE",0);	
					}
					else if(message_flag==2)
					{
						//__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
						//UART_Terminal_DMATran("Error download!\r\n");
						WM_SendMessageNoPara(hWin2,WM_USER);// сообщение окну hWin2 верхнего слоя layer 1 перерисовыаться и очистить символ update
						WM_DeleteWindow(progbar);
						MESSAGEBOX_Create("CRC ERROR!","ERROR",0);	
					}
					else if(message_flag==3)
					{
						//__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
						//UART_Terminal_DMATran("Error download!\r\n");
						progbar=WM_GetDialogItem(hWin2,ID_PROGBAR_1);
						WM_DeleteWindow(progbar);
						WM_SendMessageNoPara(hWin2,WM_USER);// сообщение окну hWin2 верхнего слоя layer 1 перерисовыаться и очистить символ update
						MESSAGEBOX_Create("SIZE FIRMWARE ERROR!","ERROR",0);	
					}
					message_flag=0;
				}
			
			GUI_Delay(10);
	}

}

/*********************************************************************
*       							UART_Terminal_DMATran
**********************************************************************/
void UART_Terminal_DMATran(char *p){
	uint8_t count=0;	
	char *s=p;
	
	while((*p++)!=0x0)
		count++;
	DMA2_Stream7->PAR=(uint32_t)&USART1->DR;
	DMA2_Stream7->M0AR=(uint32_t)s;
	DMA2_Stream7->NDTR=count;
	DMA2->HIFCR |=DMA_HIFCR_CTCIF7|DMA_HIFCR_CHTIF7;
	DMA2_Stream7->CR |=DMA_SxCR_EN;	

}

/*********************************************************************
*       										crc32 table
**********************************************************************/
const uint32_t crc32_table[256]={	
		0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

/*********************************************************************
*      										crc32_check
**********************************************************************/
uint32_t crc32_check(const uint8_t *buff,uint32_t count){

	uint32_t crc=0xffffffff;
	while(count--)
		crc=(crc>>8)^crc32_table[(crc^*buff++) & 0xFF];

	return crc^0xffffffff;

}

/*************************** End of file ****************************/
