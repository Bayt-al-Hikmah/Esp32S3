#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

void app_main() {

    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_12_BIT, 
        .freq_hz          = 5000, // 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK

    };

    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {

        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .gpio_num       = 21,
        .duty           = 0,   
        .hpoint         = 0

    };
    ledc_channel_config(&ledc_channel);
    
    gpio_reset_pin(19);
    gpio_reset_pin(20);
    gpio_set_direction(19, GPIO_MODE_OUTPUT);
    gpio_set_direction(20, GPIO_MODE_OUTPUT);
    gpio_set_level(19, 0);
    gpio_set_level(20, 0);
     
    while (1) {
        gpio_set_level(19,0);
        gpio_set_level(20,1);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 2047);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(19,1);
        gpio_set_level(20,0);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 1023);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}