#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_rom_sys.h" 

void app_main(void) {
    gpio_set_direction(13, GPIO_MODE_OUTPUT);
    gpio_set_direction(12, GPIO_MODE_OUTPUT);
    gpio_set_direction(11, GPIO_MODE_OUTPUT);
    gpio_set_direction(20, GPIO_MODE_OUTPUT);
    gpio_set_direction(19, GPIO_MODE_INPUT);

    float duration, distance;

    while (1) {
        
        gpio_set_level(20, 0);
        esp_rom_delay_us(2);
        gpio_set_level(20, 1);
        esp_rom_delay_us(10);
        gpio_set_level(20, 0);

        uint32_t start_time = 0;
        uint32_t end_time = 0;

        while(gpio_get_level(19) == 0) {
            start_time = esp_timer_get_time();
        }
        
        while(gpio_get_level(19) == 1) {
            end_time = esp_timer_get_time();
        }

        duration = (float)(end_time - start_time);
        distance = (duration * 0.0343) / 2.0;

        if (distance >= 60) {  
            gpio_set_level(13, 1); 
            gpio_set_level(12, 0); 
            gpio_set_level(11, 0);
        } else if (distance >= 30) {  
            gpio_set_level(13, 0); 
            gpio_set_level(12, 1); 
            gpio_set_level(11, 0); 
        } else {  
            gpio_set_level(13, 0); 
            gpio_set_level(12, 0);  
            gpio_set_level(11, 1); 
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}