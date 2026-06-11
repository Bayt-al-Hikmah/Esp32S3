#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


void app_main(void){

	gpio_reset_pin(2);
	gpio_set_direction(2, GPIO_MODE_OUTPUT);
        while (1) {
        gpio_set_level(2, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(2, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
}