#include "wifi.h"
#include "server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ltfs.h"

void app_main(void) {

    init_littlefs();
    initialize_pins();
    wifi_init_ap();
    vTaskDelay(pdMS_TO_TICKS(10000));
    start_webserver();
}