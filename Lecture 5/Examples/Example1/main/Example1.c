#include "uart.h"

bool ledState = false;


// Sender
void app_main(void) {  
    gpio_set_direction(1, GPIO_MODE_INPUT);
    gpio_set_pull_mode(1,GPIO_PULLDOWN_ONLY);

    uart_init();

    while (1) { 
        if(gpio_get_level(1)){
            ledState ^=1;
        }
        uart_write_bytes(UART_PORT, ledState ? "ON\r\n": "OFF\r\n", ledState ? 4: 5);  
        vTaskDelay(pdMS_TO_TICKS(1000));  
    }  
}

// Receiver
/*

uint8_t data[BUF_SIZE];  
void app_main(void)  {  
    gpio_set_direction(1, GPIO_MODE_OUTPUT);
    uart_init(); 

	
	while (1) {  
		uart_read_bytes( UART_PORT, data, BUF_SIZE, pdMS_TO_TICKS(1000) );  

		if (data[0] == 'O' && data[1] == 'N'){
			gpio_set_level(1,1);
		}else if(data[0] == 'O' && data[1] == 'F' && data[2] == 'F'){
			gpio_set_level(1,0);
		}
	}  
}
*/