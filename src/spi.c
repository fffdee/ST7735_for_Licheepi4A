#include "spi.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/spi/spidev.h>
#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include "delay.h"


#define SPI_MODE01_0        (0)
#define SPI_MODE01_1        (0 | SPI_CPHA)
#define SPI_MODE01_2        (SPI_CPOL | 0)
#define SPI_MODE01_3        (SPI_CPOL | SPI_CPHA)
#define SPI_SPEED_HZ01    20000000
#define SPI_DELAY_USECS01 0
#define SPI_BITS01        8
#define SPI_MODE          SPI_MODE01_0

#define SPI_WRITE_BUFFER_SIZE 512
#define SPI_WRITE_BUFFER_NUM  100

int  spidev_fd01 = 0;

struct spi_ioc_transfer transfer01;
uint8_t spi_write_buffer_data01[SPI_WRITE_BUFFER_SIZE];
uint8_t spi_read_buffer_data01[SPI_WRITE_BUFFER_SIZE];

int32_t buffer_pos01;
struct gpiod_chip *gpiod_chip01;
struct gpiod_line_request *gpiod_dc_lineout;
struct gpiod_line_request *gpiod_reset_lineout;
struct gpiod_line_request *gpiod_cs_lineout;

struct gpiod_line_settings *gline_settings_in, *gline_settings_out;
struct gpiod_line_config   *gline_config_in, *gline_config_out;
struct gpiod_request_config *gline_request_config_in, *gline_request_config_out;
struct gpiod_line_request   *gline_request_in, *gline_request_out;

int gpiod_value01;
int offset_out[3] = {SPI_CS, SPI_DC, SPI_RESET};

void gpiod_set_out(struct gpiod_line_request *pin, int pin_num, int num);

void spi_init(){

    if ((gpiod_chip01=gpiod_chip_open("/dev/gpiochip4")) == NULL) {
        perror("gpiod_chip_open");
        exit(-1);
    }


    gline_settings_out = gpiod_line_settings_new();
    if (gpiod_line_settings_set_direction(gline_settings_out, GPIOD_LINE_DIRECTION_OUTPUT) != 0) {
      perror("gpiod_line_settings_set_direction");
    }
    gline_config_out = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(gline_config_out, offset_out, 3, gline_settings_out);
    gline_request_config_out = gpiod_request_config_new();
    gline_request_out = gpiod_chip_request_lines(gpiod_chip01, gline_request_config_out, gline_config_out);

    gpiod_reset_lineout = gline_request_out;
    gpiod_dc_lineout = gline_request_out;
    gpiod_cs_lineout = gline_request_out;

    gpiod_set_out(gline_request_out, SPI_RESET, 1);
    gpiod_set_out(gline_request_out, SPI_CS, 1);
    gpiod_set_out(gline_request_out, SPI_DC, 1);

    gpiod_set_out(gpiod_reset_lineout, SPI_RESET, 1);
    delay_ms(100);
    gpiod_set_out(gpiod_reset_lineout, SPI_RESET, 0);
    delay_ms(200);
    gpiod_set_out(gpiod_reset_lineout, SPI_RESET, 1);
    delay_ms(200);


    spi_open();

    int ret;
    uint32_t mode;
    uint8_t  bits;
    uint32_t speed;

    mode = SPI_MODE;
    bits = SPI_BITS01;
    speed = SPI_SPEED_HZ01;

    ret = ioctl(spidev_fd01, SPI_IOC_RD_MODE32, &mode);
    if(!ret)
        ret = ioctl(spidev_fd01, SPI_IOC_WR_MODE32, &mode);
    if(!ret)
        ret = ioctl(spidev_fd01, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if(!ret)
        ret = ioctl(spidev_fd01, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if(!ret)
        ret = ioctl(spidev_fd01, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if(!ret)
        ret = ioctl(spidev_fd01, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

}


void gpiod_close() {
    if (gpiod_chip01 != 0) {
        gpiod_chip_close(gpiod_chip01);
    }
}

void gpiod_set_out(struct gpiod_line_request *pin, int pin_num, int num) {
    if ((gpiod_value01=gpiod_line_request_set_value(pin, pin_num, num)) == -1) {
        perror("gpiod_line_set_value");
    }
}


void spi_open() {

    spidev_fd01 = open("/dev/spidev2.0", O_RDWR);

}

void spi_close() {
    if (spidev_fd01 != 0) {
        close(spidev_fd01);
        spidev_fd01 = 0;
    }
}


void spi_begin() {
    gpiod_set_out(gpiod_cs_lineout, SPI_CS, 0);
}

void spi_end() {
    gpiod_set_out(gpiod_cs_lineout, SPI_CS, 1);
}


void spi_write(uint8_t spi_send_byte) {

    uint8_t spi_read_byte = 0;
    int ret;

    transfer01.tx_buf        = (unsigned long)(&spi_send_byte);
    transfer01.rx_buf        = (unsigned long)(&spi_read_byte);
    transfer01.len           = 1;
    transfer01.delay_usecs   = SPI_DELAY_USECS01;
    transfer01.speed_hz      = SPI_SPEED_HZ01;
    transfer01.bits_per_word = SPI_BITS01;
    transfer01.cs_change     = 0;

    ret = ioctl(spidev_fd01, SPI_IOC_MESSAGE(1), &transfer01);
    if (ret < 1) perror("ioctl SPI_IOC_MESSAGE");
}

uint8_t spi_read(uint8_t spi_send_byte) {

    uint8_t spi_read_byte = 0;
    int     ret;

    transfer01.tx_buf        = (unsigned long)(&spi_send_byte);
    transfer01.rx_buf        = (unsigned long)(&spi_read_byte);
    transfer01.len           = 1;
    transfer01.delay_usecs   = SPI_DELAY_USECS01;
    transfer01.speed_hz      = SPI_SPEED_HZ01;
    transfer01.bits_per_word = SPI_BITS01;
    transfer01.cs_change     = 0;

    ret = ioctl(spidev_fd01, SPI_IOC_MESSAGE(1), &transfer01);
    if (ret < 1) perror("ioctl SPI_IOC_MESSAGE");

    return (spi_read_byte &  0xff);
}


void spi_write_buffer_spi_write(uint8_t *write_buf, uint8_t *read_buf, int32_t size) {

    uint8_t spi_read_byte = 0;
    int ret;

    transfer01.tx_buf        = (unsigned long)(write_buf);
    transfer01.rx_buf        = (unsigned long)(read_buf);
    transfer01.len           = size;
    transfer01.delay_usecs   = SPI_DELAY_USECS01;
    transfer01.speed_hz      = SPI_SPEED_HZ01;
    transfer01.bits_per_word = SPI_BITS01;
    transfer01.cs_change     = 0;

    ret = ioctl(spidev_fd01, SPI_IOC_MESSAGE(1), &transfer01);
    if (ret < 1) perror("ioctl SPI_IOC_MESSAGE");
}
void spi_write_buffer(uint8_t byte) {
    spi_write_buffer_data01[buffer_pos01++] = byte;
    if (buffer_pos01 > SPI_WRITE_BUFFER_NUM) {
        spi_write_buffer_spi_write(spi_write_buffer_data01, spi_read_buffer_data01, buffer_pos01);
        buffer_pos01 = 0;
    }
}

void spi_write_buffer_flush() {
    spi_write_buffer_spi_write(spi_write_buffer_data01, spi_read_buffer_data01, buffer_pos01);
    buffer_pos01 = 0;
}

