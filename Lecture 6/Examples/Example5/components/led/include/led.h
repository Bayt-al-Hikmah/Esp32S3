#ifndef LED_H
#define LED_H


#include "driver/gpio.h"
#include "semaphores.h"

#define LED_GPIO 7
void led_toggle_task(void *pvParameters);

#endif
