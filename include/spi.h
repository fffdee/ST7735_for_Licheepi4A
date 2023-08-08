#ifndef _SPI_H__
#define _SPI_H__

#define     SPI_CS       3
#define     SPI_DC       4
#define     SPI_RESET    5

#include <stdint.h>


void spi_init();
void gpiod_close();
void spi_close();
void spi_open();
void spi_begin();
void spi_end();
void spi_write(uint8_t spi_send_byte);
uint8_t spi_read(uint8_t spi_send_byte);
void spi_write_buffer_spi_write(uint8_t *write_buf, uint8_t *read_buf, int32_t size);
void spi_write_buffer_flush();
void spi_write_buffer(uint8_t byte);

#endif



