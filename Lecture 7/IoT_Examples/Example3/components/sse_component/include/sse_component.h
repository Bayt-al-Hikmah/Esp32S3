#ifndef SSE_H  
#define SSE_H  

#include "esp_http_server.h"
#include <string.h>
#include "freertos/FreeRTOS.h"  
#include "freertos/task.h"  
#include "semaphores.h"



typedef struct{
    httpd_handle_t mySocketHD;
    int mySocketFD;
}arguments;


typedef struct{
    arguments argument;
    bool state;
} data;



extern data _data;

void send_sse_message(void *pvParameters);


#endif