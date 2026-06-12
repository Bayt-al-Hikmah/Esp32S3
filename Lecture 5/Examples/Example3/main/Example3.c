#include "spi_master.h"

bool ledState = false;

void app_main(void){

    gpio_set_direction(1, GPIO_MODE_INPUT);
    gpio_set_pull_mode(1, GPIO_PULLDOWN_ONLY); 

    spi_master_init();
    
    while (1) {        
        if (gpio_get_level(1)) {            
            ledState ^= 1;        
        }        
        
        spi_transaction_t t;
        memset(&t, 0, sizeof(t));
        t.length = 8 * 4; 
        
        if (ledState) {            
            t.tx_buffer = "ON\0\0";        
        } else {            
            t.tx_buffer = "OFF\0";        
        } 
        
        spi_device_transmit(spi_handle, &t);
        vTaskDelay(pdMS_TO_TICKS(1000));    
    }
}



/*
 main.c slave code

#include "spi_slave.h"
void app_main(void) {    
    gpio_set_direction(1, GPIO_MODE_OUTPUT);

    spi_slave_init();

    char recvbuf[4] = {0};
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));
    
    t.length = 8 * 4; 
    t.rx_buffer = recvbuf;

    while (1) {        
        
        spi_slave_transmit(SPI_HOST, &t, portMAX_DELAY);
        
        if (recvbuf[0] == 'O' && recvbuf[1] == 'N') {            
            gpio_set_level(1, 1);        
        } else if (recvbuf[0] == 'O' && recvbuf[1] == 'F' && recvbuf[2] == 'F') {
            gpio_set_level(1, 0);       
        }       
        vTaskDelay(pdMS_TO_TICKS(100));    
    }
}
*/