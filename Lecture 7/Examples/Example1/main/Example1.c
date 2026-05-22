#include <stdio.h>
#include "wifi.h"
#include "server.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {

    gpio_set_direction(10, GPIO_MODE_OUTPUT);
    gpio_set_direction(11, GPIO_MODE_OUTPUT);
    gpio_set_direction(12, GPIO_MODE_OUTPUT);

    wifi_init_ap();
    vTaskDelay(pdMS_TO_TICKS(10000));
    start_webserver();
}