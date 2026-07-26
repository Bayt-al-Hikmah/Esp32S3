#include "server.h"


esp_err_t hello_get_handler(httpd_req_t *req) {
    const char* resp_str = "Hello from ESP32-S3!";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    
    return ESP_OK;
}

httpd_uri_t hello_uri = {
    .uri      = "/hello",
    .method   = HTTP_GET,
    .handler  = hello_get_handler,
    .user_ctx = NULL
};

httpd_handle_t start_webserver(void) {
    httpd_handle_t server = NULL;

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &hello_uri);
        return server;
    }

    return NULL; 
}
