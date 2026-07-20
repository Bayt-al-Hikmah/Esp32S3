#include "led.h"


void led_toggle_task(void *pvParameters) {
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    int state = 0;

    while (1) {
        if (xSemaphoreTake(xGuiSemaphore, portMAX_DELAY) == pdPASS) {
            state = !state;
            gpio_set_level(LED_GPIO, state);
        }
    }
}
