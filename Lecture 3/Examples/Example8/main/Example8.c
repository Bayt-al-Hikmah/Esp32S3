#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

void app_main() {
   
    gpio_reset_pin(7);
    gpio_set_direction(7, GPIO_MODE_OUTPUT);

    
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, .atten = ADC_ATTEN_DB_12
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config);

    int ldrValue = 0;

    while (1) {
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &ldrValue);
        
        
        if (ldrValue > 1600) {
            gpio_set_level(7, 1); 
        } else {
            gpio_set_level(7, 0); 
        }   
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}