#include "sse_component.h"




data _data={
    .state = false,
    .argument = {
        .mySocketFD = 0,
        .mySocketHD = NULL
    },

};

void send_sse_message(void *pvParameters) {
    while (1) {
    if (xSemaphoreTake(xGuiSemaphore, portMAX_DELAY) == pdPASS) {
       if(_data.state){
            httpd_socket_send(_data.argument.mySocketHD, _data.argument.mySocketFD, "data: No obstacle\n\n", strlen("data: No obstacle\n\n"), 0);
         } else{
            httpd_socket_send(_data.argument.mySocketHD, _data.argument.mySocketFD, "data: Obstacle\n\n", strlen("data: Obstacle\n\n"), 0);
       }
    }
    }
}
