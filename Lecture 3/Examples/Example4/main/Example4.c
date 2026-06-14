#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

bool lightState = false;

void app_main(void){

    gpio_reset_pin(1);
    gpio_reset_pin(10);
    

    gpio_set_direction(1, GPIO_MODE_OUTPUT); 
    gpio_set_direction(10, GPIO_MODE_INPUT);  

    gpio_set_pull_mode(10,GPIO_PULLDOWN_ONLY);

    while (1) {
        if(gpio_get_level(10) == 0){
            lightState ^=1;
            gpio_set_level(1,lightState);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        } 
        
    }
}
