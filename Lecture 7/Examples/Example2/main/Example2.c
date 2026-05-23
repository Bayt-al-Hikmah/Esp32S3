#include <stdio.h>
#include "driver/ledc.h"

#include "wifi.h"
#include "server.h"
void app_main(void){

     ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_12_BIT, 
        .freq_hz          = 50, // 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };

    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .gpio_num       = 2,
        .duty           = 0,  
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    wifi_init_ap();  
    start_webserver();

}
