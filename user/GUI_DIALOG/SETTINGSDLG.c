#include "stm32f4xx_hal.h"
#include "DIALOG.h"


WM_HWIN Settings(void);
WM_HWIN hWin_set;
extern void Touch_calibration(void);

extern GUI_COLOR window1color;
extern GUI_COLOR window2color;

extern uint8_t lcd_bright;

extern WM_HWIN hWin0,hWin2;
extern GUI_PID_STATE State;

extern UART_HandleTypeDef huart1;

extern uint8_t Layer;
extern uint8_t Input_Device;
extern volatile uint8_t timeupdate_flag;

//const char *_uart_speed[]={"9600","19200","38400","57600","115200"};
uint32_t speed[]={9600,19200,38400,57600,115200};
/*********************************************************************
*
*       Defines
*
**********************************************************************/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x01)

#define ID_SLIDER_BRIGHT	(GUI_ID_USER + 0x2)
#define ID_TEXT_BRIGHT		(GUI_ID_USER + 0x3)
#define ID_EDIT_BRIGHT		(GUI_ID_USER + 0x4)

#define ID_SLIDER_UART1		(GUI_ID_USER + 0x5)
#define ID_TEXT_UART1			(GUI_ID_USER + 0x6)
#define ID_EDIT_UART1			(GUI_ID_USER + 0x7)

#define ID_SLIDER_R		(GUI_ID_USER + 0x8)
#define ID_SLIDER_G		(GUI_ID_USER + 0x9)
#define ID_SLIDER_B		(GUI_ID_USER + 0xA)
#define ID_EDIT_R			(GUI_ID_USER + 0xB)
#define ID_EDIT_G			(GUI_ID_USER + 0xC)
#define ID_EDIT_B			(GUI_ID_USER + 0xD)

#define ID_TEXT_WIN1		(GUI_ID_USER + 0xE)
#define ID_TEXT_WIN2		(GUI_ID_USER + 0xF)
#define ID_RADIO_WIN		(GUI_ID_USER + 0x10)

#define ID_ICON_0 		(GUI_ID_USER + 0x11)


#define ID_BUTTON_TOUCHCALIB	   (GUI_ID_USER + 0x16)

/****************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "SETTINGS", ID_FRAMEWIN_0 , 150, 10, 280, 250, 0, 0x0, 0 },
 
	{ ICONVIEW_CreateIndirect, NULL, ID_ICON_0 , 2, 120, 70, 40, 0, 0x0, 0 },
	
	{ SLIDER_CreateIndirect, NULL, ID_SLIDER_BRIGHT, 90, 10, 100, 20, 0,0, 0 },
	{ TEXT_CreateIndirect, "BRIGHTNESS", ID_TEXT_BRIGHT, 2, 10, 85, 20, 0, 0x0,0},
	{ EDIT_CreateIndirect, NULL, ID_EDIT_BRIGHT, 200, 10, 30, 20, 0, 0x2,0},
	
	{ SLIDER_CreateIndirect, NULL, ID_SLIDER_UART1, 95, 35, 100, 20, 0,0, 0 },
	{ TEXT_CreateIndirect, "UART1 Speed", ID_TEXT_UART1, 2, 35, 85, 20, 0, 0x0,0},
	{ EDIT_CreateIndirect, NULL, ID_EDIT_UART1, 200, 35, 50, 20, 0, 0x2,0},
	
	
	{ SLIDER_CreateIndirect, NULL, ID_SLIDER_R, 95, 75, 128, 20, 0,0, 0 },
	{ SLIDER_CreateIndirect, NULL, ID_SLIDER_G, 95, 100, 128, 20, 0,0, 0 },
	{ SLIDER_CreateIndirect, NULL, ID_SLIDER_B, 95, 125, 128, 20, 0,0, 0 },
	{ EDIT_CreateIndirect, NULL, ID_EDIT_R, 230, 75, 30, 20, 0, 0x2,0},
	{ EDIT_CreateIndirect, NULL, ID_EDIT_G, 230, 100, 30, 20, 0, 0x2,0},
	{ EDIT_CreateIndirect, NULL, ID_EDIT_B, 230, 125, 30, 20, 0, 0x2,0},
		
	{ RADIO_CreateIndirect,NULL,ID_RADIO_WIN,2,80,80,40,0,0x0,0},
	
	
	{ BUTTON_CreateIndirect, "Touch Calibration", ID_BUTTON_TOUCHCALIB, 90, 190, 90, 25, 0, 0x0, 0 },
};

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*    
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
  int     NCode;
  int     Id;
	uint8_t i,r,g,b;
	
	switch (pMsg->MsgId) {
			case WM_DELETE:
				RTC->BKP2R=window1color;
				RTC->BKP3R=window2color;
				hWin_set=0;
				WM_Paint(hWin0);
			break;	
			case WM_TOUCH://WM_PID_STATE_CHANGED:
				if(Input_Device)
					Layer=1;
			break;	
			case WM_INIT_DIALOG:
				hItem = pMsg->hWin;
				//FRAMEWIN_SetClientColor(hItem,GUI_LIGHTGRAY);
				FRAMEWIN_SetTitleHeight(hItem, 25);
				FRAMEWIN_SetFont(hItem,GUI_FONT_16_ASCII);
				FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT,0);
				FRAMEWIN_SetMoveable(hItem,1);	
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_ICON_0);
				ICONVIEW_SetBkColor(hItem,ICONVIEW_CI_BK,window1color);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_BRIGHT);
				TEXT_SetTextColor(hItem,0x00608030);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_SLIDER_BRIGHT);
				SLIDER_SetRange(hItem,0,100);			
				SLIDER_SetNumTicks(hItem,11);
				SLIDER_SetValue(hItem,lcd_bright);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_BRIGHT);
				EDIT_SetBkColor(hItem, EDIT_CI_ENABLED, GUI_LIGHTYELLOW);
				EDIT_SetDecMode(hItem,lcd_bright,0,100,0,GUI_EDIT_NORMAL);
				EDIT_SetValue(hItem,lcd_bright);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_UART1);
				TEXT_SetTextColor(hItem,0x00608030);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_SLIDER_UART1);
				SLIDER_SetRange(hItem,0,4);			
				SLIDER_SetNumTicks(hItem,5);
				i=0;
				while(huart1.Init.BaudRate!=speed[i])
					i++;
				SLIDER_SetValue(hItem,i);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_UART1);
				EDIT_SetBkColor(hItem, EDIT_CI_ENABLED, GUI_LIGHTYELLOW);
				EDIT_SetDecMode(hItem,speed[i],9600,115200,0,GUI_EDIT_NORMAL);
				EDIT_SetValue(hItem,speed[i]);
				
				r=(uint8_t)window1color;//&0x000000FF;				
				g=(uint8_t)(window1color>>8);	
				b=(uint8_t)(window1color>>16);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_SLIDER_R);
				SLIDER_SetRange(hItem,0,255);			
				SLIDER_SetNumTicks(hItem,9);
				SLIDER_SetValue(hItem,r);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_R);
				EDIT_SetBkColor(hItem, EDIT_CI_ENABLED, GUI_LIGHTRED);
				EDIT_SetDecMode(hItem,r,0,255,0,GUI_EDIT_NORMAL);
				EDIT_SetValue(hItem,r);
				
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_SLIDER_G);
				SLIDER_SetRange(hItem,0,255);			
				SLIDER_SetNumTicks(hItem,9);
				SLIDER_SetValue(hItem,g);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_G);
				EDIT_SetBkColor(hItem, EDIT_CI_ENABLED, GUI_GREEN);
				EDIT_SetDecMode(hItem,g,0,255,0,GUI_EDIT_NORMAL);
				EDIT_SetValue(hItem,g);
				
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_SLIDER_B);
				SLIDER_SetRange(hItem,0,255);			
				SLIDER_SetNumTicks(hItem,9);
				SLIDER_SetValue(hItem,b);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_B);
				EDIT_SetBkColor(hItem, EDIT_CI_ENABLED, GUI_LIGHTBLUE);
				EDIT_SetDecMode(hItem,b,0,255,0,GUI_EDIT_NORMAL);
				EDIT_SetValue(hItem,b);
								
				hItem=WM_GetDialogItem(pMsg->hWin,ID_RADIO_WIN);
				RADIO_SetText(hItem,"Win1 color",0);
				RADIO_SetText(hItem,"Win2 color",1);
				RADIO_SetValue(hItem,0);
				
				
			break;
			case WM_NOTIFY_PARENT:
				Id    = WM_GetId(pMsg->hWinSrc);
				NCode = pMsg->Data.v;
				switch(Id) {
					case ID_SLIDER_BRIGHT: 
						switch(NCode) {
							case WM_NOTIFICATION_VALUE_CHANGED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_SLIDER_BRIGHT);
								lcd_bright=SLIDER_GetValue(hItem);
								hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_BRIGHT);
								EDIT_SetValue(hItem,lcd_bright);
								TIM13->CCR1=lcd_bright;
							break;
							}
					break;
					case ID_SLIDER_UART1: 
						switch(NCode) {
							case WM_NOTIFICATION_VALUE_CHANGED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_SLIDER_UART1);
								i=SLIDER_GetValue(hItem);
								hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_UART1);
								if(i==0)
								{
									EDIT_SetMaxLen(hItem,4);
									EDIT_SetValue(hItem,speed[i]);
								}
								else if(i==4)
								{
									EDIT_SetMaxLen(hItem,6);
									EDIT_SetValue(hItem,speed[i]);
								
								}
								else
								{
									EDIT_SetMaxLen(hItem,5);
									EDIT_SetValue(hItem,speed[i]);
								
								}
								huart1.Init.BaudRate=speed[i];
								huart1.Instance=USART1;
								HAL_UART_Init(&huart1);
								
							break;
							}
					break;
					case ID_SLIDER_R: 
						switch(NCode) {
							case WM_NOTIFICATION_VALUE_CHANGED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_SLIDER_R);
								r=SLIDER_GetValue(hItem);
								hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_R);
								EDIT_SetValue(hItem,r);
								hItem=WM_GetDialogItem(pMsg->hWin, ID_RADIO_WIN);
								if(RADIO_GetValue(hItem)==0)
								{
									window1color=(window1color&0xFFFFFF00)|r;
									hItem=WM_GetDialogItem(pMsg->hWin, ID_ICON_0);
									ICONVIEW_SetBkColor(hItem,ICONVIEW_CI_BK,window1color);
								}
								else
								{
									window2color=(window2color&0xFFFFFF00)|r;
									hItem=WM_GetDialogItem(pMsg->hWin, ID_ICON_0);
									ICONVIEW_SetBkColor(hItem,ICONVIEW_CI_BK,window2color);
								}
								
							break;
							}
					break;
					case ID_SLIDER_G: 
						switch(NCode) {
							case WM_NOTIFICATION_VALUE_CHANGED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_SLIDER_G);
								g=SLIDER_GetValue(hItem);
								hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_G);
								EDIT_SetValue(hItem,g);
								hItem=WM_GetDialogItem(pMsg->hWin, ID_RADIO_WIN);
								if(RADIO_GetValue(hItem)==0)
								{
									window1color=(window1color&0xFFFF00FF)|(g<<8);
									hItem=WM_GetDialogItem(pMsg->hWin, ID_ICON_0);
									ICONVIEW_SetBkColor(hItem,ICONVIEW_CI_BK,window1color);
								}
								else
								{
									window2color=(window2color&0xFFFF00FF)|(g<<8);
									hItem=WM_GetDialogItem(pMsg->hWin, ID_ICON_0);
									ICONVIEW_SetBkColor(hItem,ICONVIEW_CI_BK,window2color);
								}
							
							break;
							}
					break;
					case ID_SLIDER_B: 
						switch(NCode) {
							case WM_NOTIFICATION_VALUE_CHANGED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_SLIDER_B);
								b=SLIDER_GetValue(hItem);
								hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_B);
								EDIT_SetValue(hItem,b);
								hItem=WM_GetDialogItem(pMsg->hWin, ID_RADIO_WIN);
								if(RADIO_GetValue(hItem)==0)
								{
									window1color=(window1color&0xFF00FFFF)|(b<<16);
									hItem=WM_GetDialogItem(pMsg->hWin, ID_ICON_0);
									ICONVIEW_SetBkColor(hItem,ICONVIEW_CI_BK,window1color);
								}
								else
								{
									window2color=(window2color&0xFF00FFFF)|(b<<16);
									hItem=WM_GetDialogItem(pMsg->hWin, ID_ICON_0);
									ICONVIEW_SetBkColor(hItem,ICONVIEW_CI_BK,window2color);
								}
								
							break;
							}
					break;
					case ID_RADIO_WIN: 
						switch(NCode) {
							case WM_NOTIFICATION_RELEASED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_RADIO_WIN);
								if(RADIO_GetValue(hItem)==0)
								{
									hItem=WM_GetDialogItem(pMsg->hWin, ID_ICON_0);
									r=(uint8_t)window1color;//&0x000000FF;				
									g=(uint8_t)(window1color>>8);	
									b=(uint8_t)(window1color>>16);
									ICONVIEW_SetBkColor(hItem,ICONVIEW_CI_BK,window1color);
								}
								else
								{
									hItem=WM_GetDialogItem(pMsg->hWin, ID_ICON_0);
									r=(uint8_t)window2color;//&0x000000FF;				
									g=(uint8_t)(window2color>>8);	
									b=(uint8_t)(window2color>>16);
									ICONVIEW_SetBkColor(hItem,ICONVIEW_CI_BK,window2color);
								}
								hItem=WM_GetDialogItem(pMsg->hWin,ID_SLIDER_R);
								SLIDER_SetValue(hItem,r);
								hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_R);
								EDIT_SetValue(hItem,r);
								
								hItem=WM_GetDialogItem(pMsg->hWin,ID_SLIDER_G);
								SLIDER_SetValue(hItem,g);
								hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_G);
								EDIT_SetValue(hItem,g);
								
								hItem=WM_GetDialogItem(pMsg->hWin,ID_SLIDER_B);
								SLIDER_SetValue(hItem,b);
								hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_B);
								EDIT_SetValue(hItem,b);
														
							break;
							}
					break;		
					case ID_BUTTON_TOUCHCALIB: 
						switch(NCode) {
							case WM_NOTIFICATION_RELEASED:
								WM_HideWindow(hWin0);
								WM_HideWindow(hWin2);
								Touch_calibration();
							
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
			
WM_HWIN Settings(void) {
	hWin_set = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin0, 0, 0);
  return hWin_set;
}

/*************************** End of file ****************************/

