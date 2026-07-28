
#ifndef HANDLERS_H  
#define HANDLERS_H 

#include "esp_http_server.h"
#include <stdbool.h>
#include "utils.h"
#include "leds.h"

esp_err_t led_handler(httpd_req_t *req);


#endif