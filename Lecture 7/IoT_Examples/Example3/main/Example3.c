#include "server.h"
#include "wifi.h"
#include "sse_component.h"
#include "ltfs.h"
#include "obstacle.h"


void app_main(void){
    xGuiSemaphore = xSemaphoreCreateBinary();
    if (xGuiSemaphore == NULL){
        return;
    }

    init_littlefs();
    init_obstacle_sensor();
    xTaskCreatePinnedToCore(send_sse_message, "Producer", 2048, NULL, 1, NULL, 1);
    
    wifi_init_ap();  

    start_webserver();
}
