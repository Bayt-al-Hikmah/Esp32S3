#ifndef FIRE_SENSOR_H  
#define FIRE_SENSOR_H 

#include "sse_handler.h"
#include "config.h"

extern QueueHandle_t fire_state;

void fire_sensor_task(void *pvParameters);
void set_fire_state(void *pvParameters);
#endif




