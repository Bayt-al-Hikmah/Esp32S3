#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void){

    gpio_reset_pin(4);
    gpio_reset_pin(20);

    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_set_direction(20, GPIO_MODE_OUTPUT);

    gpio_set_pull_mode(4,GPIO_PULLDOWN_ONLY);

    while (1) {
        if(gpio_get_level(4) == 0){
            gpio_set_level(20, 1);

        } else{
            gpio_set_level(20, 0);

        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}