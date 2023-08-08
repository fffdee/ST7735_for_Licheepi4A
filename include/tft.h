#ifndef _TFT_H__
#define _TFT_H__

/*---------------------------LCD-set--------------------------------------------*/
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111


#define DISPLAY_DIRECTION 0
/*-------------------------------------------------------------------------------*/
void Lcd_init();
void Lcd_clear(unsigned int Color);
void Gui_DrawPoint(unsigned int x,unsigned int y,unsigned int Data);
void LCD_WriteData_16Bit(unsigned int Data);
void Lcd_SetXY(unsigned int x,unsigned int y);
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end);
#endif