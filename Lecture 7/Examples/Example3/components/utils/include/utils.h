#ifndef UTILS_H  
#define UTILS_H  

#include "esp_http_server.h"
#include <string.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"  
#include "freertos/task.h"  



typedef struct{
    httpd_handle_t mySocketHD;
    int mySocketFD;
}arguments;


typedef struct{
    arguments argument;
    bool state;
} data;



extern data _data;

void init_obstacle_sensor(void);
void send_sse_message(void *pvParameters);


#endif