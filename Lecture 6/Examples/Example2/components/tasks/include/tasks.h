#ifndef TASKS_H
#define TASKS_H


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_1 4
#define LED_2 5

void blink_task_1(void *pvParameters);
void blink_task_2(void *pvParameters);

#endif
