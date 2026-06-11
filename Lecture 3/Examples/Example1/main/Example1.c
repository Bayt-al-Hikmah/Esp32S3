#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdbool.h>

bool LedState = false;

void app_main(void){

    gpio_reset_pin(4);
    gpio_reset_pin(2);
    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_set_pull_mode(4,GPIO_PULLDOWN_ONLY);

    while (1) {
        if(gpio_get_level(4)){
            LedState ^=1;
            gpio_set_level(2, LedState);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }  
    }
}