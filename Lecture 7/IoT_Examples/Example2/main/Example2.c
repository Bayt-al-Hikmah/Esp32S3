#include "wifi.h"
#include "server.h"

void app_main(void){
    init_servo();
    wifi_init_ap();  
    start_webserver();

}
