
#ifndef SERVER_H  
#define SERVER_H  

#include "esp_http_server.h"


esp_err_t led_handler(httpd_req_t *req);
esp_err_t dashboard_handler(httpd_req_t *req);


#endif