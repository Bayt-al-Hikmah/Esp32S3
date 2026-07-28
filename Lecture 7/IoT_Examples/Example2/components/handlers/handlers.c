#include "handlers.h"


esp_err_t servo_handler(httpd_req_t *req){
    char buffer[100];

    int received = httpd_req_recv(req, buffer, req->content_len);

    if (received <= 0) {
        return ESP_FAIL;
    }

    buffer[received] = '\0';
    
    int index = extract_index(buffer, "\"Angle\":\"", 9);
    int angle = extractNumber(buffer, index);
    set_angle(angle);
    httpd_resp_set_type(req, "application/json");
    const char* json_response =
        "{"
        "\"Response\":\"Ok\""
        "}";
    httpd_resp_send(req, json_response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

