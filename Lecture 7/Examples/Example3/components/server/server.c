#include "esp_http_server.h"
#include "handlers.h"
#include "server.h"  

httpd_uri_t dashboard_uri = {
    .uri      = "/",
    .method   = HTTP_GET,
    .handler  = dashboard_handler,
    .user_ctx = NULL
};

httpd_uri_t events_uri = {
    .uri      = "/events",
    .method   = HTTP_GET,
    .handler  = event_handler,
    .user_ctx = NULL
};

httpd_handle_t start_webserver(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &dashboard_uri);
        httpd_register_uri_handler(server, &events_uri);
        return server;
    }

    return NULL; 
}

