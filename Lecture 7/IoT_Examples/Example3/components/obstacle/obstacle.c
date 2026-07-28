#include "obstacle.h"


static void IRAM_ATTR set_state(void *pvParameters) {
    _data.state = false;
    if(gpio_get_level(GPIO_NUM_3) == 1){
        _data.state = true;
    }  
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xGuiSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


void init_obstacle_sensor(void ){
    gpio_set_direction(GPIO_NUM_3, GPIO_MODE_INPUT);
    gpio_intr_enable(GPIO_NUM_3);
    gpio_set_pull_mode(GPIO_NUM_3, GPIO_PULLUP_ONLY);
    gpio_install_isr_service(0);
    gpio_set_intr_type(GPIO_NUM_3, GPIO_INTR_NEGEDGE);
    gpio_isr_handler_add(GPIO_NUM_3, set_state, NULL);
    
}