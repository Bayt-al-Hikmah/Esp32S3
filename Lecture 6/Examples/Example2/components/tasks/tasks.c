#include "tasks.h"


void blink_task_1(void *pvParameters) {
    gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(LED_1, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(LED_1, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void blink_task_2(void *pvParameters) {
    gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(LED_2, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(LED_2, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
