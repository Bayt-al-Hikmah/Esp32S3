#include <stdio.h>
#include "wifi.h"
#include "client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void){

    wifi_init_sta();
    vTaskDelay(pdMS_TO_TICKS(30000));
    fetch_weather_data();
}
