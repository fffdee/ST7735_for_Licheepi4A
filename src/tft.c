#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "spi.h"
#include "delay.h"
#include "tft.h"

//#include "gpiod.h"

extern int32_t buffer_pos01;
extern struct gpiod_chip *gpiod_chip01;
extern struct gpiod_line_request *gpiod_dc_lineout;



void delay_ms(long milli_sec);


void Lcd_clear(unsigned int Color); 
void Gui_DrawPoint(unsigned int x,unsigned int y,unsigned int Data);

void gpiod_set_out(struct gpiod_line_request *pin, int pin_num, int num);



static void gpio_dc_on() {
    gpiod_set_out(gpiod_dc_lineout, SPI_DC, 0);
}

static void gpio_dc_off() {
    gpiod_set_out(gpiod_dc_lineout, SPI_DC, 1);
}

static void writedata(uint8_t byte) {

        spi_write_buffer(byte);
}

static void writecommand(uint8_t byte) {
        gpio_dc_on();
        spi_write(byte);
        gpio_dc_off();
}





//向液晶屏写一个16位数据
void LCD_WriteData_16Bit(unsigned int Data)
{
 	writedata(Data>>8);
 	writedata(Data);
 	spi_write_buffer_flush();
}


/*************************************************
函数名：TFT_set_region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{
	
	writecommand(0x2A);
	writedata(0X00);
	writedata(x_start);
	writedata(0X00);
	writedata(x_end+2);
 	spi_write_buffer_flush();
	writecommand(0x2B);
	writedata(0X00);
	writedata(y_start);
	writedata(0X00);
	writedata(y_end+1);
 	spi_write_buffer_flush();
	
	writecommand(0x2C);
	spi_write_buffer_flush();
    
}

void Lcd_SetXY(unsigned int x,unsigned int y)
{
     
  	Lcd_SetRegion(x,y,x,y);
  
}

void Gui_DrawPoint(unsigned int x,unsigned int y,unsigned int Data)
{
	Lcd_SetRegion(x,y,x+1,y+1);
   
	LCD_WriteData_16Bit(Data);
    
    
}    

void Lcd_clear(unsigned int Color)               
{	
   unsigned int i,m;
   spi_begin();
   Lcd_SetRegion(0,0,127,159);
   writecommand(0x2C);
   for(i=0;i<128;i++){
        for(m=0;m<160;m++)
        {	
            LCD_WriteData_16Bit(Color);
        } 
   }
      
}


void Lcd_init() {

    spi_begin();
    
    writecommand(0x11);
    spi_write_buffer_flush();
    delay_ms(120);
    writecommand(0xB1);
    writedata(0x01);
    writedata(0x2C);
    writedata(0x2D);
    spi_write_buffer_flush();

    writecommand(0xB2);
    writedata(0x01);
    writedata(0X2C);
    writedata(0X2D);
    spi_write_buffer_flush();

    writecommand(0xB3);
    writedata(0x01);
    writedata(0x2C);
    writedata(0X2D);
    writedata(0X01);
    writedata(0X2C);
    writedata(0X2D);
    spi_write_buffer_flush();

    writecommand(0xB4);
    writedata(0x07);
    spi_write_buffer_flush();

    writecommand(0xC0);
    writedata(0xA2);
    writedata(0x02);
    writedata(0x84);
    spi_write_buffer_flush();
    
    writecommand(0xC1);
    writedata(0xC5);
    spi_write_buffer_flush();

    writecommand(0xC2);
    writedata(0x0A);
    writedata(0x00);
    spi_write_buffer_flush();

    writecommand(0xC3);
    writedata(0x8A);
    writedata(0x2A);
    writecommand(0xC4);
    writedata(0x8A);
    writedata(0xEE);
    spi_write_buffer_flush();
        
    writecommand(0xC5);
    writedata(0x0E);
    spi_write_buffer_flush();


    writecommand(0xE0);
    writedata(0x0F);
    writedata(0x1A);
    writedata(0x0F);
    writedata(0x18);
    writedata(0x2F);
    writedata(0x28);
    writedata(0x20);
    writedata(0x22);
    writedata(0x1F);
    writedata(0x1B);
    writedata(0x23);
    writedata(0x37);
    writedata(0x00);
    writedata(0x07);
    writedata(0x02);
    writedata(0x10);
    spi_write_buffer_flush();
    writecommand(0xE1);
    writedata(0x0F);
    writedata(0x1B);
    writedata(0x0F);
    writedata(0x17);
    writedata(0x33);
    writedata(0x2C);
    writedata(0x29);
    writedata(0x23);
    writedata(0x30);
    writedata(0x30);
    writedata(0x39);
    writedata(0x3F);
    writedata(0x00);
    writedata(0x07);
    writedata(0x03);
    writedata(0x10);
    spi_write_buffer_flush();

    writecommand(0x2A);
    writedata(0x00);
    writedata(0x00);
    writedata(0x00);
    writedata(0x7F);
    spi_write_buffer_flush();

    writecommand(0x2B);
    writedata(0x00);
    writedata(0x00);
    writedata(0x00);
    writedata(0x9F);
    spi_write_buffer_flush();
    
    writecommand(0xF0);
    writedata(0x01);
    spi_write_buffer_flush();

    writecommand(0xF6);
    writedata(0x00);
    spi_write_buffer_flush();

    writecommand(0x3A);
    writedata(0x05);
    spi_write_buffer_flush();
    
    writecommand(0x29);
    spi_write_buffer_flush();
    

}



uint16_t color565(uint32_t colr, uint32_t colg, uint32_t colb) {
    uint16_t color;
    color = (uint16_t)(((colr & 0xff) >> 3) << 11) | (((colg & 0xff) >> 2) << 5) | ((colb & 0xff) >> 3);
    return color;
}





