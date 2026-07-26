#include "server.h"

esp_err_t json_handler(httpd_req_t *req){
    const char* json_response =
        "{"
        "\"temperature\":24,"
        "\"humidity\":60"
        "}";

    httpd_resp_set_type(req, "application/json");

    httpd_resp_send(req, json_response, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}


httpd_uri_t json_uri = {
    .uri      = "/",
    .method   = HTTP_GET,
    .handler  = json_handler,
    .user_ctx = NULL
};

httpd_handle_t start_webserver(void) {
    httpd_handle_t server = NULL;

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &json_uri);
        return server;
    }

    return NULL;
}
