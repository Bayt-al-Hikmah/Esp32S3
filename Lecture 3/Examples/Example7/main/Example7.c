#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"

void app_main() {
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE, 
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_12_BIT, 
        .freq_hz = 5000, 
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE, 
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0, 
        .gpio_num = 9, 
        .duty = 0, 
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
    
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, 
        .atten = ADC_ATTEN_DB_12 
    };
    
    adc_oneshot_new_unit(&init_config1, &adc1_handle);
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config);

    int potValue = 0;

    while (1) {
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &potValue);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, potValue);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}