#include <stdio.h>
#include "camera.h"
#include "server.h"
#include "wifi.h"

void app_main(void){
    
    gpio_set_direction(LED_PIN , GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN , 0);
    camera_init();

    wifi_init_ap();  
    start_webserver();

    
}
