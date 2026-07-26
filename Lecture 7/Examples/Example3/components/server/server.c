#include "server.h"



esp_err_t webpage_handler(httpd_req_t *req){
    const char* html_page =
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>ESP32 Web Server</title>"
        "</head>"
        "<body>"
        "<h1>ESP32-S3 Web Server</h1>"
        "<p>Hello from the ESP32!</p>"
        "</body>"
        "</html>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}


httpd_uri_t hello_uri = {
    .uri      = "/hello",
    .method   = HTTP_GET,
    .handler  = webpage_handler,
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
