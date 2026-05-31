#include <stdio.h>

#include "config.h"
#include "server.h"
#include "wifi.h"
#include "obstacle.h"
#include "fire_sensor.h"
#include "moisture_sensor.h"
#include "sse_handler.h"
#include "light_sensor.h"
void app_main(void){
    
    

    fire_state = xQueueCreate(10, sizeof(int));
    obstacle_state = xQueueCreate(10, sizeof(int));

    sse_mutex = xSemaphoreCreateMutex(); 
    wifi_init_ap();  
    start_webserver();
    setup();
    xTaskCreatePinnedToCore(fire_sensor_task, "Fire sensor task", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(light_sensor_task, "Light sensor task", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(moisture_sensor_task, "Moisture sensor task", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(obstacle_sensor_task, "Obstacle sensor task", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(handel_sse_task, "SSE handler task", 2048, NULL, 1, NULL, 1);
    
}
