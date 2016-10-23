
#include "stm32f4xx_hal.h"
#include "DIALOG.h"


WM_HWIN CreateTIME_DATE(void);


WM_HWIN hWin_time;
extern WM_HWIN hWin1,hWin2;
extern GUI_PID_STATE State;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;
extern RTC_HandleTypeDef hrtc;

extern uint8_t Layer;

static uint8_t hour,minute;
static uint8_t day,month,year;



const char *_hours[]={"00","01","02","03","04","05","06","07","08","09",
												"10","11","12","13","14","15","16","17",
												"18","19","20","21","22","23",NULL};
const char *_minute[]={"00","01","02","03","04","05","06","07","08","09",
												"10","11","12","13","14","15","16","17",
												"18","19","20","21","22","23","24","25",
												"26","27","28","29","30","31","32","33",
												"34","35","36","37","38","39","40","41",
												"42","43","44","45","46","47","48","49",
												"50","51","52","53","54","55","56","57","58","59",NULL};
													
const char *_days[]={"01","02","03","04","05","06","07","08","09","10",
											"11","12","13","14","15","16","17","18","19","20",
											"21","22","23","24","25","26","27","28","29","30","31",NULL};
										
const char *_months[]={"January","February","March","April","May","June",
												"July","August","September","October","November","December",NULL};

const char *_years[]={"2000","2001","2002","2003","2004","2005","2006","2007","2008","2009","2010",
											"2011","2012","2013","2014","2015","2016","2017","2018","2019","2020","2021",
											"2022","2023","2024","2025","2026","2027","2028","2029","2030",NULL};
/*********************************************************************
*
*       Defines
*
**********************************************************************/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x0F)
#define ID_LISTWHEEL_0		(GUI_ID_USER + 0x11)
#define ID_LISTWHEEL_1		(GUI_ID_USER + 0x12)
#define ID_LISTWHEEL_2		(GUI_ID_USER + 0x13)
#define ID_LISTWHEEL_3		(GUI_ID_USER + 0x14)
#define ID_LISTWHEEL_4		(GUI_ID_USER + 0x15)
#define ID_BUTTON_SAVE	   (GUI_ID_USER + 0x16)

/****************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "TIME_DATE", ID_FRAMEWIN_0 , 150, 60, 280, 200, 0, 0x0, 0 },
 	{ LISTWHEEL_CreateIndirect, NULL, ID_LISTWHEEL_0, 10, 20, 30, 83, 0, 0x0, 0 },
	{ LISTWHEEL_CreateIndirect, NULL, ID_LISTWHEEL_1, 45, 20, 30, 83, 0, 0x0, 0 },
	{ LISTWHEEL_CreateIndirect, NULL, ID_LISTWHEEL_2, 100, 20, 30, 83, 0, 0x0, 0 },
	{ LISTWHEEL_CreateIndirect, NULL, ID_LISTWHEEL_3, 135, 20, 70, 83, 0, 0x0, 0 },
	{ LISTWHEEL_CreateIndirect, NULL, ID_LISTWHEEL_4, 210, 20, 40, 83, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "SAVE", ID_BUTTON_SAVE, 200, 110, 50, 30, 0, 0x0, 0 },
};

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*    
*/

static void _cbDialog(WM_MESSAGE * pMsg) {
  
	WM_HWIN hItem;
  int     NCode;
  int     Id;
	switch (pMsg->MsgId) {
			case WM_DELETE:
				hWin_time=0;
			break;	
			case WM_TOUCH://WM_PID_STATE_CHANGED:
				Layer=1;
			break;	
			case WM_INIT_DIALOG:
				hItem = pMsg->hWin;
				//FRAMEWIN_SetClientColor(hItem,GUI_LIGHTGRAY);
				FRAMEWIN_SetTitleHeight(hItem, 25);
				FRAMEWIN_SetFont(hItem,GUI_FONT_16_ASCII);
				FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT,0);
				FRAMEWIN_SetMoveable(hItem,1);	
											
				hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
				LISTWHEEL_SetText(hItem,_hours);
				LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
				LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
			  LISTWHEEL_SetSnapPosition(hItem,32);
				LISTWHEEL_SetPos(hItem,sTime.Hours);
				LISTWHEEL_SetVelocity(hItem,3);
				LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
							
				hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1);
				LISTWHEEL_SetText(hItem,_minute);
				LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
				LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
				LISTWHEEL_SetSnapPosition(hItem,32);
				LISTWHEEL_SetPos(hItem,sTime.Minutes);
				LISTWHEEL_SetVelocity(hItem,3);
				LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2);
				LISTWHEEL_SetText(hItem,_days);
				LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
				LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
				LISTWHEEL_SetSnapPosition(hItem,32);
				LISTWHEEL_SetPos(hItem,(sDate.Date-1));
				LISTWHEEL_SetVelocity(hItem,3);
				LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_3);
				LISTWHEEL_SetText(hItem,_months);
				LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
				LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
				LISTWHEEL_SetSnapPosition(hItem,32);
				LISTWHEEL_SetPos(hItem,(sDate.Month-1));
				LISTWHEEL_SetVelocity(hItem,3);
				LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
							
				hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_4);
				LISTWHEEL_SetText(hItem,_years);
				LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
				LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
				LISTWHEEL_SetSnapPosition(hItem,32);
				LISTWHEEL_SetPos(hItem,sDate.Year);
				LISTWHEEL_SetVelocity(hItem,3);
				LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
							
			break;
			case WM_NOTIFY_PARENT:
				Id    = WM_GetId(pMsg->hWinSrc);
				NCode = pMsg->Data.v;
				switch(Id) {
					case ID_LISTWHEEL_0: 
						switch(NCode) {
							case WM_NOTIFICATION_SEL_CHANGED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
								hour=LISTWHEEL_GetPos(hItem);
								LISTWHEEL_SetSel(hItem,hour);
								WM_Paint(hItem);
							break;
							}
					break;
					case ID_LISTWHEEL_1: 
						switch(NCode) {
							case WM_NOTIFICATION_SEL_CHANGED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1);
								minute=LISTWHEEL_GetPos(hItem);
								LISTWHEEL_SetSel(hItem,minute);
								WM_Paint(hItem);
							break;
							}
					break;
					case ID_LISTWHEEL_2: 
						switch(NCode) {
							case WM_NOTIFICATION_SEL_CHANGED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2);
								day=LISTWHEEL_GetPos(hItem);
								LISTWHEEL_SetSel(hItem,day);
								WM_Paint(hItem);
							break;
							}
					break;
					case ID_LISTWHEEL_3: 
						switch(NCode) {
							case WM_NOTIFICATION_SEL_CHANGED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_3);
								month=LISTWHEEL_GetPos(hItem);
								LISTWHEEL_SetSel(hItem,month);
								WM_Paint(hItem);
							break;
							}
					break;		
					case ID_LISTWHEEL_4: 
						switch(NCode) {
							case WM_NOTIFICATION_SEL_CHANGED:
								hItem=WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_4);
								year=LISTWHEEL_GetPos(hItem);
								LISTWHEEL_SetSel(hItem,year);
								WM_Paint(hItem);
							break;
							}
					break;		
					case ID_BUTTON_SAVE: 
						switch(NCode) {
							case WM_NOTIFICATION_RELEASED:
								sTime.Hours=hour;
								sTime.Minutes=minute;
								sTime.Seconds=0;
								HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);//RTC_SetTime(RTC_Format_BIN, &RTC_Time);
								sDate.Year=year;
								sDate.Month=month+1;
								sDate.Date=day+1;
								HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
								WM_SendMessageNoPara(hWin2,WM_USER);
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
			
WM_HWIN CreateTIME_DATE(void) {
	
	/*hWin_time=FRAMEWIN_CreateEx(150,60,280,200,hWin1,WM_CF_SHOW,FRAMEWIN_CF_MOVEABLE,ID_FRAMEWIN_0,"TIME_DATE",_cbDialog);
	FRAMEWIN_SetClientColor(hWin_time,GUI_LIGHTGRAY);
	FRAMEWIN_SetTitleHeight(hWin_time, 25);
	FRAMEWIN_SetFont(hWin_time,GUI_FONT_16_ASCII);
	FRAMEWIN_AddCloseButton(hWin_time, FRAMEWIN_BUTTON_RIGHT,0);*/
	
	hWin_time = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin1, 0, 0);
  return hWin_time;
}

/*************************** End of file ****************************/

