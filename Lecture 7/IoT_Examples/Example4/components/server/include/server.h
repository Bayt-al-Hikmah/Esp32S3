

#ifndef SERVER_H  
#define SERVER_H  

#include "esp_http_server.h"

#include "handlers.h"

httpd_handle_t start_webserver(void);

#endif