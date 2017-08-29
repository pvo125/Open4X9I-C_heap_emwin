

#include "stm32f4xx_hal.h"
#include "DIALOG.h"

WM_HWIN CreateALARM(void);

const char *_alarm_action[]={"","START PWM","STOP PWM","START ADC","STOP ADC"};

extern const char *_hours[];/*"00","01","02","03","04","05","06","07","08","09",
												"10","11","12","13","14","15","16","17",
												"18","19","20","21","22","23",NULL};*/
extern const char *_minute[];/*={"00","01","02","03","04","05","06","07","08","09",
												"10","11","12","13","14","15","16","17",
												"18","19","20","21","22","23","24","25",
												"26","27","28","29","30","31","32","33",
												"34","35","36","37","38","39","40","41",
												"42","43","44","45","46","47","48","49",
												"50","51","52","53","54","55","56","57","58","59",NULL};*/
													
extern const char *_days[];/*={"01","02","03","04","05","06","07","08","09","10",
											"11","12","13","14","15","16","17","18","19","20",
											"21","22","23","24","25","26","27","28","29","30","31",NULL};*/

uint32_t ALARMA_ACTION;
uint32_t ALARMB_ACTION;

WM_HWIN hWin_alarm;
extern WM_HWIN hWin0;
extern RTC_AlarmTypeDef sAlarmA,sAlarmB;		   	  									
extern RTC_HandleTypeDef hrtc;
											
extern const GUI_BITMAP *Icons2[];											
											
char Alarm_A_B;									
int maskA,maskB;											
static uint8_t hour,minute;
static uint16_t day;										
/*********************************************************************
*
*       Defines
*
**********************************************************************/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x0F)
#define ID_DROPDOWN_0     (GUI_ID_USER + 0x10)

#define ID_LISTWHEEL_0		(GUI_ID_USER + 0x11)
#define ID_LISTWHEEL_1		(GUI_ID_USER + 0x12)
#define ID_LISTWHEEL_2		(GUI_ID_USER + 0x13)

#define ID_BUTTON_SAVE	   (GUI_ID_USER + 0x16)
#define ID_RADIO_0     (GUI_ID_USER + 0x17)
#define ID_DROPDOWN_1     (GUI_ID_USER + 0x18)
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "ALARM", ID_FRAMEWIN_0 , 120, 90, 280, 210, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, NULL, ID_DROPDOWN_0, 10, 120, 75, 35, 0, 0x0, 0 },
	{ DROPDOWN_CreateIndirect, NULL, ID_DROPDOWN_1, 180, 30, 80, 70, 0, 0x0, 0 },
	{ RADIO_CreateIndirect, NULL, ID_RADIO_0, 100, 120, 85, 40, 0, 0x0, 0 },
	{ LISTWHEEL_CreateIndirect, NULL, ID_LISTWHEEL_0, 10, 20, 30, 83, 0, 0x0, 0 },
	{ LISTWHEEL_CreateIndirect, NULL, ID_LISTWHEEL_1, 45, 20, 30, 83, 0, 0x0, 0 },
	{ LISTWHEEL_CreateIndirect, NULL, ID_LISTWHEEL_2, 120, 20, 30, 83, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "SAVE", ID_BUTTON_SAVE, 200, 120, 50, 30, 0, 0x0, 0 },
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
 	
	uint8_t i;
	WM_HWIN hItem;
  int     NCode;
  int     Id;
	switch (pMsg->MsgId) {
			case WM_DELETE:
				HAL_RTC_GetAlarm(&hrtc,&sAlarmA, RTC_ALARM_A, RTC_FORMAT_BIN);
				HAL_RTC_GetAlarm(&hrtc,&sAlarmB, RTC_ALARM_B, RTC_FORMAT_BIN);
				Alarm_A_B=0;
				hWin_alarm=0;
			break;	
			case WM_INIT_DIALOG:
				if(sAlarmA.AlarmMask==RTC_ALARMMASK_NONE)
					maskA=0;
				else if(sAlarmA.AlarmMask==RTC_ALARMMASK_DATEWEEKDAY)
					maskA=1;
				hItem = pMsg->hWin;
				//FRAMEWIN_SetClientColor(hItem,GUI_LIGHTGRAY);
				FRAMEWIN_SetTitleHeight(hItem, 25);
				FRAMEWIN_SetFont(hItem,GUI_FONT_16_ASCII);
				FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT,0);
				FRAMEWIN_SetMoveable(hItem,1);	
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
				RADIO_SetText(hItem,"ONCE",0);
				RADIO_SetText(hItem,"EVERY DAY",1);
				RADIO_SetValue(hItem,maskA);
								
				ALARMA_ACTION=HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0);
				ALARMB_ACTION=HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
				hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1);
				for(i=0;i<5;i++)
					DROPDOWN_AddString(hItem, _alarm_action[i]);
				DROPDOWN_SetSel(hItem,ALARMA_ACTION);
				
				
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
				DROPDOWN_AddString(hItem, "ALARM_A");
				DROPDOWN_AddString(hItem, "ALARM_B");
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
				LISTWHEEL_SetText(hItem,_hours);
				LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
				LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
			  LISTWHEEL_SetSnapPosition(hItem,32);
				LISTWHEEL_SetPos(hItem,sAlarmA.AlarmTime.Hours);
				LISTWHEEL_SetVelocity(hItem,3);
				LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);		
					
				hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1);
				LISTWHEEL_SetText(hItem,_minute);
				LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
				LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
				LISTWHEEL_SetSnapPosition(hItem,32);
				LISTWHEEL_SetPos(hItem,sAlarmA.AlarmTime.Minutes);
				LISTWHEEL_SetVelocity(hItem,3);
				LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2);
				LISTWHEEL_SetText(hItem,_days);
				LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
				LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
				LISTWHEEL_SetSnapPosition(hItem,32);
				LISTWHEEL_SetPos(hItem,sAlarmA.AlarmDateWeekDay);
				LISTWHEEL_SetVelocity(hItem,3);
				LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
				if(maskA==1)
					{
						WM_DisableWindow(hItem);
						LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTRED);
					}
			break;
			case WM_NOTIFY_PARENT:
				Id    = WM_GetId(pMsg->hWinSrc);
				NCode = pMsg->Data.v;
				switch(Id) {
					case ID_DROPDOWN_0: // Notifications sent by 'Dropdown'
						switch(NCode) {
							case WM_NOTIFICATION_SEL_CHANGED:
							hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
							Alarm_A_B=DROPDOWN_GetSel(hItem);
							if(Alarm_A_B==0)
								{
									if(sAlarmA.AlarmMask==RTC_ALARMMASK_NONE)
										maskA=0;
									else if(sAlarmA.AlarmMask==RTC_ALARMMASK_DATEWEEKDAY)
										maskA=1;
									hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
									RADIO_SetValue(hItem,maskA);
									
									hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
									LISTWHEEL_SetText(hItem,_hours);
									LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
									LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
									LISTWHEEL_SetSnapPosition(hItem,32);
									LISTWHEEL_SetPos(hItem,sAlarmA.AlarmTime.Hours);
									LISTWHEEL_SetVelocity(hItem,3);
									LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);	
									
									hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1);
									LISTWHEEL_SetText(hItem,_minute);
									LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
									LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
									LISTWHEEL_SetSnapPosition(hItem,32);
									LISTWHEEL_SetPos(hItem,sAlarmA.AlarmTime.Minutes);
									LISTWHEEL_SetVelocity(hItem,3);
									LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
									
									hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2);
									if(maskA==1)
									{	
										WM_DisableWindow(hItem);
										LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTRED);
									}
									else
									{		
										LISTWHEEL_SetText(hItem,_days);
										LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
										LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
										LISTWHEEL_SetSnapPosition(hItem,32);
										LISTWHEEL_SetPos(hItem,sAlarmA.AlarmDateWeekDay);
										LISTWHEEL_SetVelocity(hItem,3);
										LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
										WM_EnableWindow(hItem);
									}
									hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1);
									DROPDOWN_SetSel(hItem,ALARMA_ACTION);		
								}
							else
								{
									if(sAlarmB.AlarmMask==RTC_ALARMMASK_NONE)
										maskB=0;
									else if(sAlarmB.AlarmMask==RTC_ALARMMASK_DATEWEEKDAY)
										maskB=1;
									hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
									RADIO_SetValue(hItem,maskB);
									
									
									hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
									LISTWHEEL_SetText(hItem,_hours);
									LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
									LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
									LISTWHEEL_SetSnapPosition(hItem,32);
									LISTWHEEL_SetPos(hItem,sAlarmB.AlarmTime.Hours);
									LISTWHEEL_SetVelocity(hItem,3);
									LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
									
									hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1);
									LISTWHEEL_SetText(hItem,_minute);
									LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
									LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
									LISTWHEEL_SetSnapPosition(hItem,32);
									LISTWHEEL_SetPos(hItem,sAlarmB.AlarmTime.Minutes);
									LISTWHEEL_SetVelocity(hItem,3);
									LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
								
									hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2);
									if(maskB==1)
									{	
										WM_DisableWindow(hItem);
										LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTGREEN);
									}
									else
									{	
										LISTWHEEL_SetText(hItem,_days);
										LISTWHEEL_SetTextAlign(hItem,GUI_TA_HCENTER);
										LISTWHEEL_SetFont(hItem,GUI_FONT_16_ASCII);
										LISTWHEEL_SetSnapPosition(hItem,32);
										LISTWHEEL_SetPos(hItem,sAlarmB.AlarmDateWeekDay);
										LISTWHEEL_SetVelocity(hItem,3);
										LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
										WM_EnableWindow(hItem);
									}
									hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1);
									DROPDOWN_SetSel(hItem,ALARMB_ACTION);
								}		
							break;
							}
					break;
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
					case ID_RADIO_0: 
						switch(NCode) {
							case WM_NOTIFICATION_CLICKED:
							hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
							Alarm_A_B=DROPDOWN_GetSel(hItem);
							hItem=WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);	
							if(Alarm_A_B==0)
								maskA=RADIO_GetValue(hItem);
							else
								maskB=RADIO_GetValue(hItem);	
							hItem=WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2);
							if(Alarm_A_B==0)
							{		if(maskA==0)
									{
									sAlarmA.AlarmMask=RTC_ALARMMASK_NONE ;
									LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
									WM_EnableWindow(hItem);
									}
									else if(maskA==1)
									{
									sAlarmA.AlarmMask=RTC_ALARMMASK_DATEWEEKDAY;
									LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTRED);
									WM_DisableWindow(hItem);
									}
							}
							else 
							{
								if(maskB==0)
									{
									sAlarmB.AlarmMask=RTC_ALARMMASK_NONE ;
									LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTYELLOW);
									WM_EnableWindow(hItem);
									}
								else if(maskB==1)
									{
									sAlarmB.AlarmMask=RTC_ALARMMASK_DATEWEEKDAY;
									LISTWHEEL_SetBkColor(hItem,LISTWHEEL_CI_UNSEL,GUI_LIGHTGREEN);
									WM_DisableWindow(hItem);
									}
							}
							break;
							}
					break;				
					case ID_DROPDOWN_1: 
						switch(NCode) {
							case WM_NOTIFICATION_SEL_CHANGED:
								hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1);
								if(Alarm_A_B==0)
									ALARMA_ACTION=DROPDOWN_GetSel(hItem);
								else
									ALARMB_ACTION=DROPDOWN_GetSel(hItem);
							break;
							}	
					break;			
					case ID_BUTTON_SAVE: 
						switch(NCode) {
							case WM_NOTIFICATION_CLICKED:
								  if(Alarm_A_B==0)
									{	
										if((RTC->CR&RTC_CR_ALRAE)==RTC_CR_ALRAE)
										{
											//GUI_SelectLayer(0);
											GUI_MessageBox("DISABLE ALARM_A", "Message", 0);
											//GUI_SelectLayer(1);
											break;
										}	
										HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, ALARMA_ACTION);
										sAlarmA.AlarmTime.Hours=hour;
										sAlarmA.AlarmTime.Minutes=minute;
										sAlarmA.AlarmTime.Seconds=0;
										sAlarmA.AlarmDateWeekDay=day+1;
										sAlarmA.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_DATE;
										sAlarmA.Alarm = RTC_ALARM_A;
										HAL_RTC_SetAlarm_IT(&hrtc,&sAlarmA,RTC_FORMAT_BIN);
										hItem = WM_GetDialogItem(hWin0, GUI_ID_USER + 0x0A);
										WM_DeleteWindow(hItem);
										hItem=ICONVIEW_CreateEx(300,2,34,34,hWin0,WM_CF_SHOW|WM_CF_HASTRANS,0,(GUI_ID_USER + 0x0A),24,24);
										ICONVIEW_SetBkColor(hItem, ICONVIEW_CI_SEL, GUI_DARKBLUE|0x80000000);
										ICONVIEW_AddBitmapItem(hItem,Icons2[0],"");
									}	
								 else
									{
										if((RTC->CR&RTC_CR_ALRBE)==RTC_CR_ALRBE)
										{
											//GUI_SelectLayer(0);
											GUI_MessageBox("DISABLE ALARM_B", "Message", 0);
											//GUI_SelectLayer(1);
											break;
										}	
										HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, ALARMB_ACTION);
										sAlarmB.AlarmTime.Hours=hour;
										sAlarmB.AlarmTime.Minutes=minute;
										sAlarmB.AlarmTime.Seconds=0;
										sAlarmB.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_DATE;
										sAlarmB.AlarmDateWeekDay=(day+1);
										sAlarmA.Alarm = RTC_ALARM_B;
										HAL_RTC_SetAlarm_IT(&hrtc,&sAlarmB,RTC_FORMAT_BIN);
										hItem = WM_GetDialogItem(hWin0, GUI_ID_USER + 0x0B);
										WM_DeleteWindow(hItem);
										hItem=ICONVIEW_CreateEx(336,2,34,34,hWin0,WM_CF_SHOW|WM_CF_HASTRANS,0,(GUI_ID_USER + 0x0B),24,24);
										ICONVIEW_SetBkColor(hItem, ICONVIEW_CI_SEL, GUI_DARKBLUE|0x80000000);
										ICONVIEW_AddBitmapItem(hItem,Icons2[1],"");
									}
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
			
WM_HWIN CreateALARM(void) {
  hWin_alarm = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin0, 0, 0);
  return hWin_alarm;
}

/*************************** End of file ****************************/

