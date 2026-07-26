#ifndef CONFIG_H  
#define CONFIG_H  

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/queue.h"

extern adc_oneshot_unit_handle_t adc1_handle;
void setup();


#endif