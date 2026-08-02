#ifndef SERVER_H  
#define SERVER_H  

#include "esp_http_server.h"
#include "static.h"
#include "web_socket.h"

httpd_handle_t start_webserver(void);

#endif