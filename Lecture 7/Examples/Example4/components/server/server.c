#include "server.h"



esp_err_t form_handler(httpd_req_t *req){
    const char* html_page =
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>ESP32 Web Server</title>"
        "</head>"
        "<body>"
        "<h1>ESP32-S3 Web Server</h1>"
        "<form action='/submit' method='post'>"
        "<input type='text' name='led' />"
        "<br>"
        "<input type'text' name='motor' />"
        "<br>"
        "<input type='submit' />"
        "</form>"
        "</body>"
        "</html>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}


esp_err_t post_handler(httpd_req_t *req){
    char buffer[100];

    int received = httpd_req_recv(req, buffer, req->content_len);

    if (received <= 0) {
        return ESP_FAIL;
    }

    buffer[received] = '\0';

    printf("Received Data: %s\n", buffer);

    httpd_resp_send(req, "Data Received", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

httpd_uri_t form_uri = {
    .uri      = "/",
    .method   = HTTP_GET,
    .handler  = form_handler,
    .user_ctx = NULL
};
httpd_uri_t post_uri = {
    .uri = "/submit",
    .method = HTTP_POST,
    .handler = post_handler,
    .user_ctx = NULL
};

httpd_handle_t start_webserver(void) {
    httpd_handle_t server = NULL;

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &form_uri);
        httpd_register_uri_handler(server, &post_uri);
        return server;
    }

    return NULL;
}
