
#ifndef HANDLERS_H  
#define HANDLERS_H  

#include "esp_http_server.h"
#include "servo.h"
#include "utils.h"

esp_err_t servo_handler(httpd_req_t *req);


#endif