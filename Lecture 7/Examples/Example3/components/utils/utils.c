#include "utils.h"


data _data={
    .state = false,
    .argument = {
        .mySocketFD = 0,
        .mySocketHD = NULL
    },

};

void set_state(void *pvParameters) {
    _data.state = false;
    if(gpio_get_level(GPIO_NUM_4) == 1){
        _data.state = true;
    }  
}

void init_obstacle_sensor(void ){
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT);
    gpio_intr_enable(GPIO_NUM_4);
    gpio_install_isr_service(0);
    gpio_set_intr_type(GPIO_NUM_4, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(GPIO_NUM_4, set_state, NULL);
    
}

void send_sse_message(void *pvParameters) {
    while (1) {
       if(_data.state){
        httpd_socket_send(_data.argument.mySocketHD, _data.argument.mySocketFD, "data: no obstacle\n\n", strlen("data: no obstacle\n\n"), 0);
         } else{
            httpd_socket_send(_data.argument.mySocketHD, _data.argument.mySocketFD, "data: obstacle\n\n", strlen("data: obstacle\n\n"), 0);
       }
       vTaskDelay(pdMS_TO_TICKS(100));
    }
}