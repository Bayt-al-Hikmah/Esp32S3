#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main(void){
    gpio_reset_pin(5);
    gpio_reset_pin(2);
    gpio_reset_pin(20);
    
    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    gpio_set_direction(20, GPIO_MODE_OUTPUT);
    gpio_set_direction(5, GPIO_MODE_INPUT);
    
    gpio_set_pull_mode(5,GPIO_PULLDOWN_ONLY);
    while (1) {
        while(gpio_get_level(5)){
            gpio_set_level(20, 1);
            gpio_set_level(2, 1);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            gpio_set_level(2, 0);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }  
         gpio_set_level(20, 0);
    }
}