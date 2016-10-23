#include <stm32f4xx.h>
#include "GUI.h"
#include "DIALOG.h"
#include "stdlib.h"
void Touch_GetData(void);
void Touch_calibration(void);
#define TOUCH_PRESSED		1
#define TOUCH_UNPRESSED	0
GUI_PID_STATE State;
extern uint8_t Layer;
extern LTDC_HandleTypeDef hltdc;
extern uint8_t backlight_count;
extern WM_HWIN hWin0,hWin2;
uint16_t number_click;

static float Xd[]={0.0f,240.0f,30.0f,450.0f,450.0f,30.0f};
static float Yd[]={0.0f,136.0f,30.0f,30.0f,242.0f,242.0f};
static uint16_t Xt[6];
static uint16_t Yt[6];
static float a,b,c,d,e,Rt;

static uint32_t X1,X2,X3,Y1,Y2,Y3;
static float detX1,detX2,detX3,detY1,detY2,detY3;
static float det;


static float A=0.143228f,B=-0.00176398f,C=-40.16921f,D=0.00007187f,E=-0.093f,F=319.33f;

void Touch_SendCMD(uint8_t Data)
{
	SPI2->DR=Data;
	while((SPI2->SR&SPI_SR_RXNE)!=SPI_SR_RXNE)		{}	//while((SPI2->SR&SPI_SR_BSY)==SPI_SR_BSY)	{}
}
uint16_t Touch_GetResult(void)
{
	uint16_t temp;
		temp=SPI2->DR;		/* Clear RXNE SPI2 */
	SPI2->DR=0x0;
	while((SPI2->SR&SPI_SR_RXNE)!=SPI_SR_RXNE)		{}
	temp=SPI2->DR>>3;
			return temp;
}
uint16_t _ReadZ2(void)
{
	uint16_t  z2;
	SPI2->DR=0xC4;
	while((SPI2->SR&SPI_SR_RXNE)!=SPI_SR_RXNE)	{} //while((SPI2->SR&SPI_SR_BSY)==SPI_SR_BSY)	{}
	z2=SPI2->DR;		/* Clear RXNE SPI2 */
	
	SPI2->DR=0x0;
	while(!(SPI2->SR&SPI_SR_RXNE))	{}											
	z2=SPI2->DR>>3;
	return z2;																										
}

uint16_t _ReadZ1(void)
{
	uint16_t  z1;
	SPI2->DR=0xB4;
	while((SPI2->SR&SPI_SR_RXNE)!=SPI_SR_RXNE)	{} //while((SPI2->SR&SPI_SR_BSY)==SPI_SR_BSY)	{}	
	z1=SPI2->DR;		/* Clear RXNE SPI2 */
	
	SPI2->DR=0x0;
	while(!(SPI2->SR&SPI_SR_RXNE))	{}											
	z1=SPI2->DR>>3;
	return z1;																										
}

uint16_t _TouchPositionX(void)
{
	uint16_t  x;
			
	SPI2->DR=0xD0;
	while((SPI2->SR&SPI_SR_RXNE)!=SPI_SR_RXNE)	{}//while((SPI2->SR&SPI_SR_BSY)==SPI_SR_BSY)	{}	
	x=SPI2->DR;		/* Clear RXNE SPI2 */
	
	//while((XPT2046_BUSY_GPIO_Port->IDR&GPIO_IDR_IDR_2)==GPIO_IDR_IDR_2) {}	
				
	SPI2->DR=0x00;
	while((SPI2->SR&SPI_SR_RXNE)!=SPI_SR_RXNE)	{}//while(!(SPI2->SR&SPI_SR_RXNE))	{}											
	x=SPI2->DR>>3;
	return x;																										
																															
	
}	

uint16_t _TouchPositionY(void)
{
	uint16_t y;
		
	
	SPI2->DR=0x90;
	while((SPI2->SR&SPI_SR_RXNE)!=SPI_SR_RXNE)	{}//while((SPI2->SR&SPI_SR_BSY)==SPI_SR_BSY)	{}	
	y=SPI2->DR;		/* Clear RXNE SPI2 */			  	
		
	//while((XPT2046_BUSY_GPIO_Port->IDR&GPIO_IDR_IDR_2)==GPIO_IDR_IDR_2) {}
	
	SPI2->DR=0x00;
	while(!(SPI2->SR&SPI_SR_RXNE))	{}
	y=SPI2->DR>>3;															
	return y;
}
uint8_t _TouchIsPresset(void)
{
	if(( XPT2046_IRQ_GPIO_Port->IDR&GPIO_IDR_IDR_3)==RESET)   //GPIO_ReadInputDataBit ( GPIOB,GPIO_Pin_11 ) == RESET )
		return 1;
	else
		return 0;
}
void Touch_GetData(void)
{			uint16_t Xd,Yd,Xt,Xt1,Xt2,Yt,Yt1,Yt2;
			uint16_t z1,z2;
						
		if((GPIOE->IDR&GPIO_IDR_IDR_3)==RESET)
			{
				backlight_count=0;
				TIM7->CNT=0;
				TIM13->CCR1=70;
				if((TIM7->CR1&TIM_CR1_CEN)!=TIM_CR1_CEN)
				{
					TIM7->CR1|=TIM_CR1_CEN;
					
				}
				XPT2046_CS_GPIO_Port->BSRR=GPIO_BSRR_BR_5;
				z1=_ReadZ1();
				z2=_ReadZ2();
				Touch_SendCMD(0xD4);
				Xt=Touch_GetResult();
				Xt1=_TouchPositionX();
				Xt2=_TouchPositionX();		//Rt=5.0f*Xt*(z2/z1-1.0f)/32760;	
				
				Touch_SendCMD(0x94);
				Yt=Touch_GetResult();	
				Yt1=_TouchPositionY();
				Yt2=_TouchPositionY();	
				XPT2046_CS_GPIO_Port->BSRR=GPIO_BSRR_BS_5;
				
				if((Xt<=Xt1)&&(Xt<=Xt2)){
						Xt=( Xt1<=Xt2) ? Xt1 : Xt2;
					}
					else 
					{
						if((Xt1<=Xt)&&(Xt1<=Xt2)){
							Xt=(Xt<=Xt2) ? Xt : Xt2;
							}
						else{
							Xt=(Xt<=Xt1) ? Xt : Xt1;
						}
					}
					if((Yt<=Yt1)&&(Yt<=Yt2)){
						Yt=( Yt1<=Yt2) ? Yt1 : Yt2;
					}
					else 
					{
						if((Yt1<=Yt)&&(Yt1<=Yt2)){
							Yt=(Yt<=Yt2) ? Yt : Yt2;
							}
						else{
							Yt=(Yt<=Yt1) ? Yt : Yt1;
						}
					}	
					Rt=5.0f*Xt/4096*(z2/z1-1.0f);
					if (Rt<=4.0f)
					{
						Xd=A*Xt+B*Yt+C;
						Yd=D*Xt+E*Yt+F;
						State.y=Yd;
						State.x=Xd;
						State.Pressed=TOUCH_PRESSED;
						GUI_TOUCH_StoreStateEx(&State);
					}
				
			}
			else
			{	
				GUI_TOUCH_GetState(&State);
				if(State.Pressed==TOUCH_PRESSED)
					{
				   State.Pressed=TOUCH_UNPRESSED;
					 GUI_TOUCH_StoreStateEx(&State);	
					}
			}
}
/*
 **
 ***
 ****/
void DrawTarget(uint16_t x,uint16_t y){
		
	GUI_DrawVLine(x,y-5,y+5);
	GUI_DrawHLine(y,x-5,x+5);
	GUI_DrawCircle(x,y,4);	
}

/*
 **
 ***
 ****/
void Touch_calibration(void){
	uint8_t i;
	
	NVIC_DisableIRQ(TIM6_DAC_IRQn);
	NVIC_DisableIRQ(TIM7_IRQn);
	NVIC_DisableIRQ(RTC_WKUP_IRQn);
	
	LCD_SetAlphaEx(1, 0);
	GUI_SelectLayer(0);	 	
	GUI_SetFont(GUI_FONT_16_ASCII);
	GUI_SetBkColor(GUI_DARKBLUE);
	GUI_Clear();
	GUI_Delay(1000);
	GUI_SetColor(GUI_WHITE);
	for(i=1;i<6;i++)
	{
		GUI_DispStringHCenterAt("PRESS",Xd[i],Yd[i]+10);
		DrawTarget(Xd[i],Yd[i]);
		while(_TouchIsPresset()==0) {}  //while((GPIOE->IDR&GPIO_IDR_IDR_3)!=GPIO_IDR_IDR_3) {}  такое выражение компилятор выкидывает.
		GUI_Delay(1500);
		XPT2046_CS_GPIO_Port->BSRR=GPIO_BSRR_BR_5;
		Xt[i]=_TouchPositionX();
		Yt[i]=_TouchPositionY();
		XPT2046_CS_GPIO_Port->BSRR=GPIO_BSRR_BS_5;
		GUI_Clear();
		GUI_Delay(2000);
		
	}
	a=Xt[1]*Xt[1]+Xt[2]*Xt[2]+Xt[3]*Xt[3]+Xt[4]*Xt[4]+Xt[5]*Xt[5];
	b=Yt[1]*Yt[1]+Yt[2]*Yt[2]+Yt[3]*Yt[3]+Yt[4]*Yt[4]+Yt[5]*Yt[5];
	c=Xt[1]*Yt[1]+Xt[2]*Yt[2]+Xt[3]*Yt[3]+Xt[4]*Yt[4]+Xt[5]*Yt[5];
	d=Xt[1]+Xt[2]+Xt[3]+Xt[4]+Xt[5];
	e=Yt[1]+Yt[2]+Yt[3]+Yt[4]+Yt[5];
	
	X1=Xt[1]*Xd[1]+Xt[2]*Xd[2]+Xt[3]*Xd[3]+Xt[4]*Xd[4]+Xt[5]*Xd[5];
	X2=Yt[1]*Xd[1]+Yt[2]*Xd[2]+Yt[3]*Xd[3]+Yt[4]*Xd[4]+Yt[5]*Xd[5];
	X3=Xd[1]+Xd[2]+Xd[3]+Xd[4]+Xd[5];
	
	Y1=Xt[1]*Yd[1]+Xt[2]*Yd[2]+Xt[3]*Yd[3]+Xt[4]*Yd[4]+Xt[5]*Yd[5];
	Y2=Yt[1]*Yd[1]+Yt[2]*Yd[2]+Yt[3]*Yd[3]+Yt[4]*Yd[4]+Yt[5]*Yd[5];
	Y3=Yd[1]+Yd[2]+Yd[3]+Yd[4]+Yd[5];
	
	det=5*(a*b-c*c)+2*c*d*e-a*e*e-b*d*d;
	detX1=5*(X1*b-X2*c)+e*(X2*d-X1*e)+X3*(c*e-b*d);
	detX2=5*(X2*a-X1*c)+d*(X1*e-X2*d)+X3*(c*d-a*e);
	detX3=X3*(a*b-c*c)+X1*(c*e-b*d)+X2*(c*d-a*e);
	detY1=5*(Y1*b-Y2*c)+e*(Y2*d-Y1*e)+Y3*(c*e-b*d);
	detY2=5*(Y2*a-Y1*c)+d*(Y1*e-Y2*d)+Y3*(c*d-a*e);
	detY3=Y3*(a*b-c*c)+Y1*(c*e-b*d)+Y2*(c*d-a*e);
	A=detX1/det;
	B=detX2/det;
	C=detX3/det;
	D=detY1/det;
	E=detY2/det;
	F=detY3/det;
	/*A=(Xd[1]*(Yt[2]-Yt[3])+Xd[2]*(Yt[3]-Yt[1])+Xd[3]*(Yt[1]-Yt[2]))/(Xt[1]*(Yt[2]-Yt[3])+Xt[2]*(Yt[3]-Yt[1])+Xt[3]*(Yt[1]-Yt[2]));
	B=(A*(Xt[3]-Xt[2])+Xd[2]-Xd[3])/(Yt[2]-Yt[3]);
	C=Xd[3]-A*Xt[3]-B*Yt[3];
	D=(Yd[1]*(Yt[2]-Yt[3])+Yd[2]*(Yt[3]-Yt[1])+Yd[3]*(Yt[1]-Yt[2]))/(Xt[1]*(Yt[2]-Yt[3])+Xt[2]*(Yt[3]-Yt[1])+Xt[3]*(Yt[1]-Yt[2]));
	E=(D*(Xt[3]-Xt[2])+Yd[2]-Yd[3])/(Yt[2]-Yt[3]);
	F=Yd[3]-D*Xt[3]-E*Yt[3];*/
	
	
	
	GUI_DispStringHCenterAt("CALIBRATION COMLETE",240,136);
	GUI_Delay(1000);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	NVIC_EnableIRQ(TIM7_IRQn);
	NVIC_EnableIRQ(RTC_WKUP_IRQn);
	GUI_SelectLayer(1);
	WM_ShowWindow(hWin0);
	WM_ShowWindow(hWin2);
	LCD_SetAlphaEx(1, 255);
}

