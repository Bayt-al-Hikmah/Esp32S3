#ifndef SSE_HANDLER_H  
#define SSE_HANDLER_H  

#include "esp_http_server.h"
#include <stdlib.h> 
#include "freertos/FreeRTOS.h"  
#include "freertos/queue.h"
#include "freertos/task.h"  
#include "freertos/semphr.h"  

#define MAX_ATTEMPS 5

typedef struct{
    httpd_handle_t mySocketHD;
    int mySocketFD;
}sse_session;

typedef struct{
    int mySocketFD;
    int failed_attemp;
}sse_log;

extern SemaphoreHandle_t sse_mutex; 
extern sse_session* SEE_SESSIONS;
extern sse_log* SEE_LOG;
extern int sse_clients_number;

void send_sse_message(char * message);
void handel_sse_task(void *pvParameters);
void add_sse(sse_session sse_client);

#endif