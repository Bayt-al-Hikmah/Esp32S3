#ifndef SPI_MASTER_H
#define SPI_MASTER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <string.h>
#include <stdbool.h>

#define SPI_MASTER_MOSI_IO 11
#define SPI_MASTER_MISO_IO 12
#define SPI_MASTER_SCLK_IO 13
#define SPI_MASTER_CS_IO 10
#define SPI_HOST SPI2_HOST

extern spi_device_handle_t spi_handle;

void spi_master_init();

#endif