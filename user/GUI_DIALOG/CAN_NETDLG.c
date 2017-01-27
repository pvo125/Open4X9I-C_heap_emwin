

#include "stm32f4xx_hal.h"
#include "DIALOG.h"
#include "CAN.h"
#include "stdlib.h"
#include "string.h"

#define NUMBER_NODE 5					// Зададим максимальное количество для CAN сети

volatile uint32_t countbyte_firmware=0;			// Глобальная переменная счетчик отправленных байт firmware  при обновлении по CAN
const char  *netname_array[]={"Open4X9I","Core4x9I","F103_KIT","32VLDisc"}; //массив указателей на имена узлов CAN сети соотв. netname_index (0,1,2...)
volatile uint8_t netname_index;
uint8_t window_index;
uint8_t count_node;
WM_HWIN WinHandle[NUMBER_NODE];		//Соответствующий стат. массив где хранятся дескрипторы окон узлов CAN сети

WM_HWIN hWin_CANNetExplore;
TREEVIEW_Handle hTree;

WM_HWIN CANNetExplore(void);
WM_HWIN CANNodeDialog(uint8_t index);

extern volatile int size;
extern volatile uint32_t TimeMS;
volatile uint8_t new_node=0;
volatile uint8_t download_complete=0;
volatile uint8_t new_firmware=0;

extern WM_HWIN hWin1;
					
extern GUI_CONST_STORAGE GUI_BITMAP bmchip;

/*********************************************************************
*
*       Defines
*
**********************************************************************/
#define ID_FRAMEWIN_1    (GUI_ID_USER + 0x20)
#define ID_TEXT_TIME	   (GUI_ID_USER + 0x22)
#define ID_TEXT_DATE	   (GUI_ID_USER + 0x23)
#define ID_TEXT_TIMER	   (GUI_ID_USER + 0x24)
#define ID_TEXT_MODE	   (GUI_ID_USER + 0x25)
#define ID_TEXT_ENABLE   (GUI_ID_USER + 0x26)
#define ID_TEXT_PHASEVAL (GUI_ID_USER + 0x27)
#define ID_TEXT_BREZVAL  (GUI_ID_USER + 0x28)
#define ID_DROPDOWN_MODE  (GUI_ID_USER + 0x29)
#define ID_DROPDOWN_ENABLE  (GUI_ID_USER + 0x2A)
#define ID_SPINBOX_VALUE_P    (GUI_ID_USER+0x2B)
#define ID_SPINBOX_VALUE_B    (GUI_ID_USER+0x2C)
#define ID_EDIT_TIME_H    (GUI_ID_USER+0x2D)
#define ID_EDIT_TIME_M    (GUI_ID_USER+0x2E)
#define ID_EDIT_DATE_D    (GUI_ID_USER+0x2F)
#define ID_EDIT_DATE_M    (GUI_ID_USER+0x30)
#define ID_EDIT_DATE_Y    (GUI_ID_USER+0x31)
#define ID_EDIT_CAN_REC    (GUI_ID_USER+0x32)
#define ID_EDIT_CAN_TEC    (GUI_ID_USER+0x33)
#define ID_EDIT_CAN_ERF    (GUI_ID_USER+0x34)

#define ID_TEXT_1    (GUI_ID_USER+0x35)
#define ID_TEXT_2    (GUI_ID_USER+0x36)
#define ID_TEXT_3    (GUI_ID_USER+0x37)
#define ID_TEXT_REC    (GUI_ID_USER+0x38)
#define ID_TEXT_TEC    (GUI_ID_USER+0x39)
#define ID_TEXT_ERF    (GUI_ID_USER+0x3A)
#define ID_BUTTON_UPDATE    (GUI_ID_USER+0x3B)
#define ID_PROGBAR_1     	(GUI_ID_USER + 0x3C)
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aCANNodeDialog[] = {
  { FRAMEWIN_CreateIndirect,NULL , ID_FRAMEWIN_1 , 20, 5, 230, 270, 0, 0x0, 15},
	{ TEXT_CreateIndirect, "Time", ID_TEXT_TIME, 15, 14, 50, 20, 0, 0x0,0},
	{ TEXT_CreateIndirect, "Date", ID_TEXT_DATE, 15, 35, 50, 20, 0, 0x0,0},
	{ TEXT_CreateIndirect, "TIMER", ID_TEXT_TIMER, 15, 55, 50, 20, 0, 0x0,0},
	{ TEXT_CreateIndirect, "Mode", ID_TEXT_MODE, 30, 75, 50, 20, 0, 0x0,0},
	{ TEXT_CreateIndirect, "Enable", ID_TEXT_ENABLE, 30, 105, 50, 20, 0, 0x0,0},
	{ TEXT_CreateIndirect, "PhaseVal", ID_TEXT_PHASEVAL, 30, 150, 70, 20, 0, 0x0,0},
	{ TEXT_CreateIndirect, "BrezVal", ID_TEXT_BREZVAL, 30, 150, 70, 20, 0, 0x0,0},
	
	{ BUTTON_CreateIndirect, "Update_firmware", ID_BUTTON_UPDATE, 5, 200, 100, 30, 0, 0x0,0},
	
	{ DROPDOWN_CreateIndirect, NULL, ID_DROPDOWN_MODE, 100, 75, 60, 30, 0, 0x0,0},
	{ DROPDOWN_CreateIndirect, NULL, ID_DROPDOWN_ENABLE, 100, 105, 70, 30, 0, 0x0,0},
	{ SPINBOX_CreateIndirect, NULL, ID_SPINBOX_VALUE_P, 100, 150, 80, 40, 0, 0x0,0},
	{ SPINBOX_CreateIndirect, NULL, ID_SPINBOX_VALUE_B, 100, 150, 80, 40, 0, 0x0,0},
	
	{ EDIT_CreateIndirect, NULL, ID_EDIT_CAN_REC, 50, 0, 25, 15, 0, 0x3,0},
	{ EDIT_CreateIndirect, NULL, ID_EDIT_CAN_TEC, 125, 0, 25, 15, 0, 0x3,0},
	{ EDIT_CreateIndirect, NULL, ID_EDIT_CAN_ERF, 200, 0, 12, 15, 0, 0x1,0},
	{ TEXT_CreateIndirect, "REC", ID_TEXT_REC, 25, 1, 20, 20, 0, 0x0,0},
	{ TEXT_CreateIndirect, "TEC", ID_TEXT_TEC, 100, 1, 20, 20, 0, 0x0,0},
	{ TEXT_CreateIndirect, "ERF", ID_TEXT_ERF, 175, 1, 20, 20, 0, 0x0,0},
	
	{ EDIT_CreateIndirect, NULL, ID_EDIT_TIME_H, 100, 15, 20, 20, 0, 0x2,0},
	{ EDIT_CreateIndirect, NULL, ID_EDIT_TIME_M, 125, 15, 20, 20, 0, 0x2,0},
	
	{ EDIT_CreateIndirect, NULL, ID_EDIT_DATE_D, 100, 35, 20, 20, 0, 0x2,0},
	{ EDIT_CreateIndirect, NULL, ID_EDIT_DATE_M, 125, 35, 20, 20, 0, 0x2,0},
	{ EDIT_CreateIndirect, NULL, ID_EDIT_DATE_Y, 150, 35, 20, 20, 0, 0x2,0},
	{ TEXT_CreateIndirect, ":", ID_TEXT_1, 120, 15, 5, 20, 0, 0x0,0},
	{ TEXT_CreateIndirect, ".", ID_TEXT_2, 120, 35, 5, 20, 0, 0x0,0},
	{ TEXT_CreateIndirect, ".", ID_TEXT_3, 145, 35, 5, 20, 0, 0x0,0},
	};

static void _cbCANNodeDialog(WM_MESSAGE * pMsg){
	CANNode_TypeDef CANNode;
	static uint8_t widget_changing=0;
	WM_HWIN hItem;
	uint32_t temp;
	uint8_t index;
	int     NCode;
  int     Id;
	PROGBAR_Handle progbar;
	
  switch (pMsg->MsgId) {
			case WM_DELETE:
				FRAMEWIN_GetUserData(pMsg->hWin,&index,1);
				WinHandle[index]=0;
			break;	
			case WM_INIT_DIALOG:
				
			memset(&CANNode,0,sizeof(CANNode));	//Инизиализируем структуру CANNode нулями
				
				hItem=pMsg->hWin;
				FRAMEWIN_SetTitleHeight(hItem, 25);
				FRAMEWIN_SetFont(hItem,GUI_FONT_16_ASCII);
				FRAMEWIN_SetText(hItem,netname_array[window_index]);
				FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT,0);
				FRAMEWIN_AddMinButton(hItem, FRAMEWIN_BUTTON_RIGHT,20);
				FRAMEWIN_SetClientColor( hItem, GUI_BLACK);
				FRAMEWIN_SetMoveable(hItem,1);
				CANNode.index=window_index;
				FRAMEWIN_SetUserData(hItem,(const void*)&CANNode,sizeof(CANNode));	// Сохраним структуру узла CAN сети в соотв. этому узлу окне
			  						
				CAN_Transmit_RemoteFrame(0x080);		 //широковещательный запрос GET_RTC  каждую секунду.
				CAN_Transmit_RemoteFrame(0x082);		 //широковещательный запрос GET_TIMER_DATA каждую секунду.
				
				hItem=WM_GetClientWindow(pMsg->hWin);	
				WM_CreateTimer(hItem,0,1000,0);				// Создадим таймер для обновления соотв. окна диалога для каждого узла.
				
				Id=strcmp((const char*)netname_array[window_index],(const char*)0xD0400020);
				if(Id || (new_firmware==0))
					{
						hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_UPDATE);
						WM_DisableWindow(hItem);	
					}
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_BREZVAL);
				WM_HideWindow(hItem);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_VALUE_B);
				WM_HideWindow(hItem);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_VALUE_P);
				SPINBOX_SetFont(hItem,GUI_FONT_16B_ASCII);
				SPINBOX_SetValue(hItem,CANNode.timerPhase_value);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_MODE);
				DROPDOWN_AddString(hItem, "PHASE");
				DROPDOWN_AddString(hItem, "BREZ");
				//DROPDOWN_SetSel(hItem,CANNode_Struct[index].timerPhaseBrez);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_ENABLE);
				DROPDOWN_AddString(hItem, "DISABLE");
				DROPDOWN_AddString(hItem, "ENABLE");
				//DROPDOWN_SetSel(hItem,CANNode_Struct[index].timer0nOff);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_1);
				TEXT_SetFont(hItem,GUI_FONT_16B_ASCII);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_2);
				TEXT_SetFont(hItem,GUI_FONT_16B_ASCII);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_3);
				TEXT_SetFont(hItem,GUI_FONT_16B_ASCII);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_REC);
				TEXT_SetFont(hItem,&GUI_Font6x8);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_TEC);
				TEXT_SetFont(hItem,&GUI_Font6x8);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_ERF);
				TEXT_SetFont(hItem,&GUI_Font6x8);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_CAN_REC);
				EDIT_SetFont(hItem,&GUI_Font6x8);
				EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,GUI_YELLOW);
				EDIT_SetDecMode(hItem,CANNode.can_err[0],0,255,0,GUI_EDIT_NORMAL);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_CAN_TEC);
				EDIT_SetFont(hItem,&GUI_Font6x8);
				EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,GUI_YELLOW);
				EDIT_SetDecMode(hItem, CANNode.can_err[1],0,255,0,GUI_EDIT_NORMAL);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_CAN_ERF);
				EDIT_SetFont(hItem,&GUI_Font6x8);
				EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,GUI_YELLOW);
				EDIT_SetDecMode(hItem, CANNode.can_err[2],0,7,0,GUI_EDIT_NORMAL);
					
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_TIME_H);
				EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,GUI_YELLOW);
				EDIT_SetDecMode(hItem,CANNode.time[0],0,23,0,GUI_EDIT_NORMAL);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_TIME_M);
				EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,GUI_YELLOW);
				EDIT_SetDecMode(hItem,CANNode.time[1],0,59,0,GUI_EDIT_NORMAL);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_DATE_D);
				EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,GUI_YELLOW);
				EDIT_SetDecMode(hItem,CANNode.date[0],1,31,0,GUI_EDIT_NORMAL);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_DATE_M);
				EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,GUI_YELLOW);
				EDIT_SetDecMode(hItem,CANNode.date[1],1,12,0,GUI_EDIT_NORMAL);
				hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_DATE_Y);
				EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,GUI_YELLOW);
				EDIT_SetDecMode(hItem,CANNode.date[2],1,30,0,GUI_EDIT_NORMAL);
				
				
				
			break;	
			case WM_TIMER:
 				FRAMEWIN_GetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));	//Когда таймер сраб. получим с соотв. окна структуру CAN узла.
				
				if(countbyte_firmware)
				{
					temp=(countbyte_firmware*100)/size;
					progbar=WM_GetDialogItem(pMsg->hWin,ID_PROGBAR_1);
					PROGBAR_SetValue(progbar,temp);
				}
				if(download_complete)
				{
					hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_UPDATE);
					WM_DisableWindow(hItem);
					download_complete=0;
					progbar=WM_GetDialogItem(pMsg->hWin,ID_PROGBAR_1);
					WM_DeleteWindow(progbar);
					
					
				}
				if(widget_changing)
				{
					WM_RestartTimer(pMsg->Data.v,1000);
					break;
				}
				if(CANNode.newmsg)						//Флаг нового сообщения. Если флаг установлен значит новые данные и перерисуем окно .
				{	
					hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_CAN_REC);
					EDIT_SetValue(hItem,CANNode.can_err[0]);
					hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_CAN_TEC);
					EDIT_SetValue(hItem,CANNode.can_err[1]);
					hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_CAN_ERF);
					EDIT_SetValue(hItem,CANNode.can_err[2]);
					
					hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_DATE_Y);
					EDIT_SetValue(hItem,CANNode.date[2]);
					hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_DATE_M);
					EDIT_SetValue(hItem,CANNode.date[1]);
					hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_DATE_D);
					EDIT_SetValue(hItem,CANNode.date[0]);
					hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_TIME_H);
					EDIT_SetValue(hItem,CANNode.time[0]);
					hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_TIME_M);
					EDIT_SetValue(hItem,CANNode.time[1]);
					if(CANNode.timerPhaseBrez)
						{	
							hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_PHASEVAL);
							WM_HideWindow(hItem);
							hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_BREZVAL);
							WM_ShowWindow(hItem);
							hItem=WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_VALUE_P);
							WM_HideWindow(hItem);   // hide spinbox_value_p
							hItem=WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_VALUE_B);
							WM_ShowWindow(hItem);   // show spinbox_value_b
							SPINBOX_SetFont(hItem,GUI_FONT_16B_ASCII);
							SPINBOX_SetValue(hItem,CANNode.timerBrez_value);
						}
					else
						{
							
							hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_BREZVAL);
							WM_HideWindow(hItem);
							hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_PHASEVAL);
							WM_ShowWindow(hItem);
							hItem=WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_VALUE_B);
							WM_HideWindow(hItem);   // hide spinbox_value_b
							hItem=WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_VALUE_P);
							WM_ShowWindow(hItem);	 // show spinbox_value_p
							SPINBOX_SetFont(hItem,GUI_FONT_16B_ASCII);
							SPINBOX_SetValue(hItem,CANNode.timerPhase_value);
						}
					hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_ENABLE);
					DROPDOWN_SetSel(hItem,CANNode.timer0nOff);
					hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_MODE);
					DROPDOWN_SetSel(hItem,CANNode.timerPhaseBrez);
					CANNode.count=0;
					CANNode.newmsg=0;
					FRAMEWIN_SetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));
					
				}
				else			// Если не было новых данных от соот. окну узла сети значит увеличим счетчик CANNode.count и сохраним структуру назад в соот. окне
				{
					CANNode.count++;
					FRAMEWIN_SetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));
					if(CANNode.count>1)		// Доп. проверка если 2 раза таймер сработал и не было новых данных от узла 
					{
						CANNode.count=0;							//
						CANNode.newmsg=0;							//
						WinHandle[CANNode.index]=0;		//
						GUI_EndDialog(pMsg->hWin,0);	//значит соединение разорвано и закрываем соответствующий диалог.
					}
			
				}
				/*Каждый раз повторяем запросы*/		
				CAN_Transmit_RemoteFrame(0x080);		 //широковещательный запрос GET_RTC  каждую секунду.
				CAN_Transmit_RemoteFrame(0x082);		 //широковещательный запрос GET_TIMER_DATA каждую секунду.
				WM_RestartTimer(pMsg->Data.v,1000);
			break;
			case WM_NOTIFY_PARENT:
				Id    = WM_GetId(pMsg->hWinSrc);
				NCode = pMsg->Data.v;
				switch(Id) {
					case ID_SPINBOX_VALUE_B: // Notifications sent by SPINBOX_VALUE
						switch(NCode) {
							case WM_NOTIFICATION_CLICKED:
								widget_changing=1;
							break;	
							case WM_NOTIFICATION_VALUE_CHANGED:
								
								//FRAMEWIN_GetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));//FRAMEWIN_GetUserData(pMsg->hWin,&index,1);
								//hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_VALUE_B);
								//CANNode.timerBrez_value=SPINBOX_GetValue(hItem);//WM_RestartTimer(pMsg->Data.v,1000);
								//FRAMEWIN_SetUserData(pMsg->hWin,&CANNode,sizeof(CANNode))
								
							break;
							case WM_NOTIFICATION_RELEASED:
								FRAMEWIN_GetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));//FRAMEWIN_GetUserData(pMsg->hWin,&index,1);
								hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_VALUE_B);
								CANNode.timerBrez_value=SPINBOX_GetValue(hItem);
								CAN_Data_TX.ID=((CANNode.index+1)<<8)|0x83;				// 0x(index+1)83 SET_TIMER_DATA
								CAN_Data_TX.DLC=4;					 
								CAN_Data_TX.Data[1]=CANNode.timerPhaseBrez;		//
								CAN_Data_TX.Data[3]=CANNode.timerBrez_value;
								CAN_Transmit_DataFrame(&CAN_Data_TX);
								FRAMEWIN_SetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));
								widget_changing=0;
							break;
							}
					break;
					case ID_SPINBOX_VALUE_P: // Notifications sent by SPINBOX_VALUE
						switch(NCode) {
							case WM_NOTIFICATION_CLICKED:
								widget_changing=1;
							break;	
							case WM_NOTIFICATION_VALUE_CHANGED:
								
								//FRAMEWIN_GetUserData(pMsg->hWin,&index,1);
								//hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_VALUE_P);
								//CANNode_Struct[index].timerPhase_value=SPINBOX_GetValue(hItem);//WM_RestartTimer(pMsg->Data.v,1000);
							break;	
							case WM_NOTIFICATION_RELEASED:
								FRAMEWIN_GetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));//FRAMEWIN_GetUserData(pMsg->hWin,&index,1);
								hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_VALUE_P);//FRAMEWIN_GetUserData(pMsg->hWin,&index,1);
								CANNode.timerPhase_value=SPINBOX_GetValue(hItem);
								CAN_Data_TX.ID=((CANNode.index+1)<<8)|0x83;				// 0x(index+1)83 SET_TIMER_DATA
								CAN_Data_TX.DLC=3;					 
								CAN_Data_TX.Data[1]=CANNode.timerPhaseBrez;		//
								CAN_Data_TX.Data[2]=CANNode.timerPhase_value;
								CAN_Transmit_DataFrame(&CAN_Data_TX);	
								FRAMEWIN_SetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));
								widget_changing=0;
							break;
							}
					break;
					case ID_DROPDOWN_MODE: // Notifications sent by DROPDOWN_MODE
						switch(NCode) {
							
							case WM_NOTIFICATION_SEL_CHANGED:
								
								FRAMEWIN_GetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));//FRAMEWIN_GetUserData(pMsg->hWin,&index,1);
								hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_MODE);
								CANNode.timerPhaseBrez=DROPDOWN_GetSel(hItem);
								CAN_Data_TX.ID=((CANNode.index+1)<<8)|0x83;		// 0x(index+1)83 set_timer
								CAN_Data_TX.DLC=4;
								CAN_Data_TX.Data[1]=CANNode.timerPhaseBrez;
								CAN_Data_TX.Data[2]=CANNode.timerPhase_value;
								CAN_Data_TX.Data[3]=CANNode.timerBrez_value;
								CAN_Transmit_DataFrame(&CAN_Data_TX);
								FRAMEWIN_SetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));
								
							break;
							}
					break;
					case ID_DROPDOWN_ENABLE: // Notifications sent by DROPDOWN_ENABLE
						switch(NCode) {
								
							case WM_NOTIFICATION_SEL_CHANGED:
								FRAMEWIN_GetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));//FRAMEWIN_GetUserData(pMsg->hWin,&index,1);
								hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_ENABLE);
								CANNode.timer0nOff=DROPDOWN_GetSel(hItem);
								if(CANNode.timer0nOff)
									CAN_Transmit_RemoteFrame(((CANNode.index+1)<<8)|0x82);   // Core4X9I 0x283 remote enable timer
								else
									CAN_Transmit_RemoteFrame(((CANNode.index+1)<<8)|0x83);	// // Core4X9I 0x283 remote disable timer	
								
								FRAMEWIN_SetUserData(pMsg->hWin,&CANNode,sizeof(CANNode));
								//widget_changing=0;
							break;
							
							}
					break;
					case ID_BUTTON_UPDATE: // Notifications sent by BUTTON_UPDATE
							switch(NCode){
								case WM_NOTIFICATION_RELEASED:
									progbar=PROGBAR_CreateEx(120,240,100,10,pMsg->hWin,WM_CF_SHOW|WM_CF_HASTRANS,PROGBAR_CF_HORIZONTAL,ID_PROGBAR_1);
									PROGBAR_SetText(progbar,"");	
									PROGBAR_SetMinMax(progbar,0,100);
									countbyte_firmware=0;
									CAN_Data_TX.ID=(CANNode.index+1)<<8|0x71;
									CAN_Data_TX.DLC=4;
									CAN_Data_TX.Data[0]=(uint8_t)size;
									CAN_Data_TX.Data[1]=(uint8_t)(size>>8);
									CAN_Data_TX.Data[2]=(uint8_t)(size>>16);
									CAN_Data_TX.Data[3]=(uint8_t)(size>>24);
									CAN_Transmit_DataFrame(&CAN_Data_TX);				// (Core4X9I 0x271) UPDATE_FIRMWARE_REQ 0x271, 0x371, 0x471
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
*
*       Defines
*
**********************************************************************/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x40)
#define ID_TREE_0     		(GUI_ID_USER + 0x41)
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aCANNetExplore[] = {
  { FRAMEWIN_CreateIndirect, "CAN_NET", ID_FRAMEWIN_0 , 120, 90, 250, 150, 0, 0x0, 0 },
	{ TREEVIEW_CreateIndirect, NULL, ID_TREE_0, 0, 0, 250, 150, 0, 0x0,0},
	};

/*********************************************************************
*
*       Public code
*/
/*********************************************************************
*
*
*
**********************************************************************/
void _Browse_CANNet_FirstEntry(void){
	TREEVIEW_ITEM_Handle hNode;
	uint8_t count=0;
	
	for(;;)
		{
			TimeMS=0;
			while((new_node==0)&&(TimeMS<5)) {}
			if(TimeMS>=5)
				//{
				//count_node=count;
				break;
			//	}
			hNode=TREEVIEW_InsertItem(hTree,TREEVIEW_ITEM_TYPE_LEAF,hNode,TREEVIEW_INSERT_BELOW,netname_array[netname_index]);
			TREEVIEW_ITEM_SetUserData(hNode,(uint32_t)(netname_index));
			#ifdef CODEFLASH
				TREEVIEW_ITEM_SetImage(hNode, TREEVIEW_BI_LEAF,&bmchip);
			#endif
			count++;
			new_node=0;
		}
}

/**************************************************************************
*
*
*
****************************************************************************/	
void	_Browse_CANNet(void){
	TREEVIEW_ITEM_Handle hNode;
	uint8_t count=0;
		
	hNode=TREEVIEW_GetItem(hTree,0,TREEVIEW_GET_FIRST);
	if(hNode)
	{
			
			while(hNode)
			{
				hNode=TREEVIEW_GetSel(hTree);	
				TREEVIEW_ITEM_Delete(hNode);
			}
	}
	for(;;)
		{
			TimeMS=0;
			while((new_node==0)&&(TimeMS<5)) {}
			if(TimeMS>=5)
				//count_node=count;
				break;
				
			hNode=TREEVIEW_InsertItem(hTree,TREEVIEW_ITEM_TYPE_LEAF,hNode,TREEVIEW_INSERT_BELOW,netname_array[netname_index]);
			TREEVIEW_ITEM_SetUserData(hNode,(uint32_t)(netname_index));
			#ifdef CODEFLASH
				TREEVIEW_ITEM_SetImage(hNode, TREEVIEW_BI_LEAF,&bmchip);
			#endif
			count++;
			new_node=0;
		}

}
/*********************************************************************
*
*/

static void _cbCANNetExplore(WM_MESSAGE * pMsg) {
 	TREEVIEW_ITEM_Handle hNode;
	WM_HWIN hItem;
  int     NCode;
  int     Id;
	
	switch (pMsg->MsgId) {
			case WM_DELETE:
				hWin_CANNetExplore=0;
				
			break;	
			case WM_INIT_DIALOG:
											
				hItem = pMsg->hWin;
				FRAMEWIN_SetTitleHeight(hItem, 25);
				FRAMEWIN_SetFont(hItem,GUI_FONT_16_ASCII);
				FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT,0);
				FRAMEWIN_SetMoveable(hItem,1);	
				
				WM_CreateTimer(WM_GetClientWindow(hItem),0,2000,0);
			
				hTree = WM_GetDialogItem(pMsg->hWin, ID_TREE_0);
				TREEVIEW_SetFont(hTree,GUI_FONT_16B_ASCII);
				TREEVIEW_SetTextIndent(hTree,30);
				TREEVIEW_SetSelMode(hTree,TREEVIEW_SELMODE_ROW);
			
				CAN_Transmit_RemoteFrame(0x088);		 //широковещательный запрос get net name
				_Browse_CANNet_FirstEntry();
				
			break;
			case WM_TIMER:
				CAN_Transmit_RemoteFrame(0x088);		 //широковещательный запрос get net name каждую секунду.
				_Browse_CANNet();
				WM_RestartTimer(pMsg->Data.v,2000);
			break;	
			case WM_NOTIFY_PARENT:
				Id    = WM_GetId(pMsg->hWinSrc);
				NCode = pMsg->Data.v;
				switch(Id) {
					case ID_TREE_0: // Notifications sent by 'TREE'
						switch(NCode) {
							
							case WM_NOTIFICATION_RELEASED:
								hTree = WM_GetDialogItem(pMsg->hWin, ID_TREE_0);
								hNode=TREEVIEW_GetSel(hTree);
								window_index=(uint8_t)TREEVIEW_ITEM_GetUserData(hNode);
								
								if(hNode)
								{
									if((WinHandle[window_index])==0)
										CANNodeDialog(window_index);	
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
			
WM_HWIN CANNetExplore(void) {
  hWin_CANNetExplore = GUI_CreateDialogBox(_aCANNetExplore, GUI_COUNTOF(_aCANNetExplore), _cbCANNetExplore, hWin1, 0, 0);
 	return hWin_CANNetExplore;

}
WM_HWIN CANNodeDialog(uint8_t index){
	
	
	TEXT_SetDefaultTextColor(GUI_YELLOW);
	
	
	WinHandle[index]=GUI_CreateDialogBox(_aCANNodeDialog, GUI_COUNTOF(_aCANNodeDialog), _cbCANNodeDialog, hWin1, 0, 0);
	
	return WinHandle[index];
}

/*************************** End of file ****************************/

