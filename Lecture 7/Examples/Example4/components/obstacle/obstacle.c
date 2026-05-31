#include "obstacle.h"


QueueHandle_t obstacle_state = NULL;

void set_obstacle_state(void *pvParameters) {
    int state = gpio_get_level(GPIO_NUM_4);
    xQueueSendFromISR(obstacle_state, &state , NULL);
}

void init_obstacle_sensor(void ){
    gpio_isr_handler_add(GPIO_NUM_4, set_obstacle_state, NULL);  
}



void obstacle_sensor_task(void *pvParameters){

    int state;
    init_obstacle_sensor();
    while(1){
         xQueueReceive(obstacle_state, &state, portMAX_DELAY);
        
         if(state){
            
            send_sse_message("event: Obstacle Sensor\r\n"
            "data: No obstacle Detected\r\n"
            "\r\n");
         }else{
            send_sse_message("event: Obstacle Sensor\r\n"
            "data: Obstacle Detected\r\n"
            "\r\n");
         }
    }
}