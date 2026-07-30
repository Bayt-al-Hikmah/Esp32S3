
#ifndef STATIC_H  
#define STATIC_H 

#include "esp_http_server.h"
esp_err_t dashboard_handler(httpd_req_t *req);
esp_err_t style_handler(httpd_req_t *req);
esp_err_t script_handler(httpd_req_t *req);

#endif