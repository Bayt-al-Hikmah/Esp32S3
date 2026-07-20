#ifndef SENSOR_H
#define SENSOR_H


#include "queue.h"

#include "esp_adc/adc_oneshot.h"
#include "driver/gpio.h"
void sensor_task(void *pvParameters);

#endif
