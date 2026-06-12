#include "spi_slave.h"

void spi_slave_init() {
    spi_bus_config_t buscfg = {
        .mosi_io_num = SPI_SLAVE_MOSI_IO,
        .miso_io_num = SPI_SLAVE_MISO_IO,
        .sclk_io_num = SPI_SLAVE_SCLK_IO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    spi_slave_interface_config_t slvcfg = {
        .mode = 0,
        .spics_io_num = SPI_SLAVE_CS_IO,
        .queue_size = 3,
        .flags = 0,
    };

    spi_slave_initialize(SPI_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);

}
