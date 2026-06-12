
#include "spi_master.h"


spi_device_handle_t spi_handle = NULL;


void spi_master_init(){

    spi_bus_config_t buscfg = {
        .mosi_io_num = SPI_MASTER_MOSI_IO,
        .miso_io_num = SPI_MASTER_MISO_IO,
        .sclk_io_num = SPI_MASTER_SCLK_IO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };

    spi_bus_initialize(SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000,           
        .mode = 0,                           
        .spics_io_num = SPI_MASTER_CS_IO,    
        .queue_size = 1,
    };
    
    
    spi_bus_add_device(SPI_HOST, &devcfg, &spi_handle);
}