
#ifndef HANDLERS_H  
#define HANDLERS_H  

#include "sse_handler.h"
#include "obstacle.h"
#include "fire_sensor.h"
#include "sse_handler.h"
#include <string.h>


esp_err_t event_handler(httpd_req_t *req);
esp_err_t dashboard_handler(httpd_req_t *req);


#endif