#ifndef OBSTACLE_H  
#define OBSTACLE_H 

#include "sse_handler.h"
#include "config.h"


extern QueueHandle_t obstacle_state;


void obstacle_sensor_task(void *pvParameters);
void set_obstacle_state(void *pvParameters);
#endif




