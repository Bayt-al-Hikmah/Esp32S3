
#ifndef HANDLERS_H  
#define HANDLERS_H  

#include "esp_http_server.h"


esp_err_t servo_handler(httpd_req_t *req);
esp_err_t dashboard_handler(httpd_req_t *req);


#endif