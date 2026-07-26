#include "fire_sensor.h"


QueueHandle_t fire_state =NULL;

void set_fire_state(void *pvParameters) {
    int state = gpio_get_level(GPIO_NUM_5);
    xQueueSendFromISR(fire_state, &state , NULL);
}

void init_fire_sensor(void ){
    gpio_isr_handler_add(GPIO_NUM_5, set_fire_state, NULL);
}



void fire_sensor_task(void *pvParameters){
    init_fire_sensor();
    int state;
    while(1){
         xQueueReceive(fire_state, &state, portMAX_DELAY);
         if(state){
            
            send_sse_message("event: Flame Sensor\r\n"
            "data: No Flame Detected\r\n"
            "\r\n");
         }else{
            send_sse_message("event: Flame Sensor\r\n"
            "data: Flame Detected\r\n"
            "\r\n");
         }
    }
}