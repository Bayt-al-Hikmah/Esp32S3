#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main() {

    gpio_reset_pin(20);
    gpio_reset_pin(19);
    gpio_set_direction(20, GPIO_MODE_OUTPUT);
    gpio_set_direction(19, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(19, 0);
        for (int i=0;i<100;i++){
          gpio_set_level(20, 1);
          vTaskDelay(pdMS_TO_TICKS(50));
          gpio_set_level(20, 0);
          vTaskDelay(pdMS_TO_TICKS(50));
        }
        vTaskDelay(pdMS_TO_TICKS(3000));
        gpio_set_level(19, 1);
        for (int i=0;i<100;i++){
          gpio_set_level(20, 1);
          vTaskDelay(pdMS_TO_TICKS(50));
          gpio_set_level(20, 0);
          vTaskDelay(pdMS_TO_TICKS(50));
        }
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}