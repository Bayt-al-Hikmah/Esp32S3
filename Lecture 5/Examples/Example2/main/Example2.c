#include "i2c_master.h"

bool ledState = false;

void app_main(void) {
    gpio_set_direction(1, GPIO_MODE_INPUT);
    gpio_set_pull_mode(1, GPIO_PULLDOWN_ONLY);  
    
    i2c_master_init();
    while (1) {        
	    if (gpio_get_level(1)) {            
		    ledState ^= 1;        
		}        
		if (ledState) {            
			i2c_master_transmit(dev_handle, (uint8_t *)"ON", 2, -1);        
		} else {            
			i2c_master_transmit(dev_handle, (uint8_t *)"OFF", 3, -1);        
		}        
		vTaskDelay(pdMS_TO_TICKS(1000));    
	}
}
/*
 //slave main.c
#include "i2c_slave.h"
void app_main(void) {
    gpio_set_direction(1, GPIO_MODE_OUTPUT);
    i2c_slave_init();
    while (1) {        
	    if (context.data[0] == 'O' && context.data[1] == 'N') {            
		    gpio_set_level(1, 1);        
	    } else if (context.data[0] == 'O' && context.data[1] == 'F' && context.data[2] == 'F') {
		    gpio_set_level(1, 0);       
		}       
		vTaskDelay(pdMS_TO_TICKS(100));    
	}
}
*/
