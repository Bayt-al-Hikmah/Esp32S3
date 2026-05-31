
#ifndef HANDLERS_H  
#define HANDLERS_H 

#include "utils.h"


esp_err_t event_handler(httpd_req_t *req);
esp_err_t dashboard_handler(httpd_req_t *req);


#endif