#include "server.h"
#include "wifi.h"
#include "utils.h"


void app_main(void){
    
    
    init_obstacle_sensor();
    xTaskCreatePinnedToCore(send_sse_message, "Producer", 2048, NULL, 1, NULL, 1);
    wifi_init_ap();  
    start_webserver();
}
