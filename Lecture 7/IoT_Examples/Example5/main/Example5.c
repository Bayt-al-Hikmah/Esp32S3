#include <stdio.h>
#include "camera.h"
#include "server.h"
#include "wifi.h"
#include "ltfs.h"

void app_main(void){
    
    init_littlefs();
    camera_init();

    wifi_init_ap();  
    start_webserver();

    
}
