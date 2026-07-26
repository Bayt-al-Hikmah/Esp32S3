#include "wifi.h"
#include "server.h"

void app_main(){

   wifi_init_ap();
   start_webserver();

}
