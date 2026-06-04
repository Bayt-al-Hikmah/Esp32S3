#include <stdio.h>
#include "camera.h"
#include "server.h"
#include "wifi.h"

void app_main(void){
    
    camera_init();

    wifi_init_ap();  
    start_webserver();

    
}
