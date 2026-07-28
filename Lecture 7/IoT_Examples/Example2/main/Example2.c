#include "wifi.h"
#include "server.h"
#include "ltfs.h"

void app_main(void){
    init_littlefs();
    init_servo();
    wifi_init_ap();  
    start_webserver();

}
