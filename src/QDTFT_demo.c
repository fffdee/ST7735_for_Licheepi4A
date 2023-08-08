
#include "GUI.h"
#include "delay.h"
#include "Picture.h"
#include "QDTFT_demo.h"
#include "tft.h"


unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};
void Redraw_Mainmenu(void)
{

	Lcd_clear(GRAY0);


	DisplayButtonUp(15,38,113,58); //x1,y1,x2,y2
	//Gui_DrawFont_GBK16(16,40,YELLOW,GRAY0,"��ɫ������");

	DisplayButtonDown(15,68,113,88); //x1,y1,x2,y2
	//Gui_DrawFont_GBK16(16,70,BLUE,GRAY0,"������ʾ����");

	DisplayButtonUp(15,98,113,118); //x1,y1,x2,y2
	//Gui_DrawFont_GBK16(16,100,RED,GRAY0,"ͼƬ��ʾ����");
	//delay_ms(1500);
}

void Num_Test(void)
{
	unsigned char i=0;
	Lcd_clear(GRAY0);
	//Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	//delay_ms(1000);
	Lcd_clear(GRAY0);

	for(i=0;i<10;i++)
	{
	Gui_DrawFont_Num32((i%3)*40,32*(i/3)+5,RED,GRAY0,Num[i+1]);
	//delay_ms(100);
	}
	
}

void Font_Test(void)
{
	Lcd_clear(GRAY0);
	//Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"������ʾ����");

	//delay_ms(1000);
	Lcd_clear(GRAY0);
	//Gui_DrawFont_GBK16(16,30,BLUE,GRAY0,"���Ӽ���");

	//Gui_DrawFont_GBK16(20,70,RED,GRAY0, "QQ-GROUP:");	
	//Gui_DrawFont_GBK16(20,100,RED,GRAY0, "878535527");
	//delay_ms(1800);	
}

void Color_Test(void)
{
	unsigned char i=1;
	Lcd_clear(GRAY0);
	
	//Gui_DrawFont_GBK16(20,10,BLUE,GRAY0,"Color Test");
	delay_ms(200);

	while(i--)
	{
		Lcd_clear(WHITE);
		Lcd_clear(BLACK);
		Lcd_clear(RED);
	  	Lcd_clear(GREEN);
	  	Lcd_clear(BLUE);
	}		
}


void showimage(const unsigned char *p) 
{
  	int i,j,k; 
	unsigned char picH,picL;
	Lcd_clear(WHITE); 
	
	for(k=0;k<4;k++)
	{
	   	for(j=0;j<3;j++)
		{	
			Lcd_SetRegion(40*j+2,40*k,40*j+39,40*k+39);		
		    for(i=0;i<40*40;i++)
			 {	
			 	picL=*(p+i*2);	
				picH=*(p+i*2+1);				
				LCD_WriteData_16Bit(picH<<8|picL);  						
			 }	
		 }
	}		
}
void QDTFT_Test_Demo(void)
{
	Lcd_init();
			
	Redraw_Mainmenu();
	Color_Test();
	Num_Test();
	//Font_Test();
	
	showimage(gImage_qq);
	Gui_Circle(60,60,20,BLUE);

	
}
