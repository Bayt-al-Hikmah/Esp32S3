#include "sse_handler.h"

SemaphoreHandle_t sse_mutex = NULL;
sse_session* SEE_SESSIONS = NULL;
sse_log* SEE_LOG = NULL;
int sse_clients_number = 0;

void send_sse_message(char * message) {
    int error_code;
    
    for (int i = 0 ;i < sse_clients_number; i++){
        
        error_code = httpd_socket_send((SEE_SESSIONS+i)->mySocketHD, (SEE_SESSIONS+i)->mySocketFD,  message,strlen(message), 0);

        if (error_code == HTTPD_SOCK_ERR_TIMEOUT || error_code == HTTPD_SOCK_ERR_FAIL){
            if ((SEE_LOG+i)-> mySocketFD == (SEE_SESSIONS+i)->mySocketFD){
                (SEE_LOG+i)->failed_attemp += 1;
            }
        }
    }
    
  
}

void handel_sse_task(void *pvParameters){

    while(1){
        xSemaphoreTake(sse_mutex, portMAX_DELAY);  
        for (int i=0;i<sse_clients_number;i++){
            if ((SEE_LOG + i)->failed_attemp > MAX_ATTEMPS){
                sse_clients_number -= 1;
                (SEE_LOG + i)->failed_attemp = (SEE_LOG + sse_clients_number)->failed_attemp;
                (SEE_LOG + i)->mySocketFD = (SEE_LOG + sse_clients_number)->mySocketFD;

                (SEE_SESSIONS + i)->mySocketHD = (SEE_SESSIONS + sse_clients_number)->mySocketHD;
                (SEE_SESSIONS + i)->mySocketFD = (SEE_SESSIONS + sse_clients_number)->mySocketFD;
            }
        }
        xSemaphoreGive(sse_mutex);  

        vTaskDelay(pdMS_TO_TICKS(30000));  
    }
}
void add_sse(sse_session sse_client){
   
    xSemaphoreTake(sse_mutex, portMAX_DELAY);  
    
    sse_session* tmp_client = malloc((sse_clients_number+1)*sizeof(sse_session));
    sse_log* tmp_log = malloc((sse_clients_number+1)*sizeof(sse_session));
     
    for (int i = 0; i < sse_clients_number; i++){
        
        (tmp_client + i)->mySocketHD = (SEE_SESSIONS + i)->mySocketHD; 
        (tmp_client + i)->mySocketFD =(SEE_SESSIONS + i)->mySocketFD;

        (tmp_log + i)->mySocketFD =(SEE_LOG + i)->mySocketFD;
        (tmp_log + i)->failed_attemp =(SEE_LOG+ i)->failed_attemp;
    }
    sse_clients_number += 1;
    (tmp_client + sse_clients_number -1)->mySocketHD=sse_client.mySocketHD;
    (tmp_client + sse_clients_number -1)->mySocketFD=sse_client.mySocketFD;
    

    (tmp_log + sse_clients_number -1)->failed_attemp = 0;
    (tmp_log + sse_clients_number -1)->mySocketFD = sse_client.mySocketFD;
    free(SEE_SESSIONS);
    SEE_SESSIONS = tmp_client;
    free(SEE_LOG);
    SEE_LOG = tmp_log;
    xSemaphoreGive(sse_mutex); 
}