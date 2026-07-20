#include "light.h"

void light_task(void *pvParameters) {
    int received_data = 0;
    gpio_reset_pin(7);
    gpio_set_direction(7, GPIO_MODE_OUTPUT);
    while (1) {
       xQueueReceive(data_queue, &received_data, portMAX_DELAY);
       if (received_data > 1600) {
            gpio_set_level(7, 1);
        } else {
            gpio_set_level(7, 0);
        }

    }
}
