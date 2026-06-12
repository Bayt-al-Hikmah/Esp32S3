#ifndef SPI_SLAVE_H
#define SPI_SLAVE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_slave.h"
#include "driver/gpio.h"
#include <string.h>

#define SPI_SLAVE_MOSI_IO 11
#define SPI_SLAVE_MISO_IO 12
#define SPI_SLAVE_SCLK_IO 13
#define SPI_SLAVE_CS_IO 10
#define SPI_HOST SPI2_HOST



void spi_slave_init();


#endif