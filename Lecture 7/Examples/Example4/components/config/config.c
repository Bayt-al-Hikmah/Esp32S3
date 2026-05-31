#include "config.h"
adc_oneshot_unit_handle_t adc1_handle=NULL;


void setup() {

    adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, .atten = ADC_ATTEN_DB_12
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config);
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_1, &config);
    


    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT);
    gpio_intr_enable(GPIO_NUM_4);
    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_INPUT);
    gpio_intr_enable(GPIO_NUM_5);

    gpio_install_isr_service(0);    

    gpio_set_intr_type(GPIO_NUM_4, GPIO_INTR_ANYEDGE);
    gpio_set_intr_type(GPIO_NUM_5, GPIO_INTR_ANYEDGE);
}