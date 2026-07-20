#include "sensor.h"

void sensor_task(void *pvParameters) {

    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, .atten = ADC_ATTEN_DB_12
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_4, &config);

    int ldrValue = 0;
    while (1) {
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_4, &ldrValue);
        xQueueSend(data_queue, &ldrValue , pdMS_TO_TICKS(100));
        vTaskDelay(pdMS_TO_TICKS(500)); 
    }
}
