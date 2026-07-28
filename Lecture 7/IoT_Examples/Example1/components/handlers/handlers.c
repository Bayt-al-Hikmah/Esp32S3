#include "handlers.h"



esp_err_t led_handler(httpd_req_t *req){
    char buffer[100];

    int received = httpd_req_recv(req, buffer, req->content_len);

    if (received <= 0) {
        return ESP_FAIL;
    }

    buffer[received] = '\0';
    
    int index = extract_index(buffer, "\"Led\":", 6);
    bool newState = 0;
    if (index != -1) {
        int ledNumber = buffer[index] - '0';
        newState = toggle_led(ledNumber);
    }
    
    httpd_resp_set_type(req, "application/json");
    if (newState){
         const char* json_response =
        "{"
        "\"NextState\":\"OFF\""
        "}";
        httpd_resp_send(req, json_response, HTTPD_RESP_USE_STRLEN);
    }else{
         const char* json_response =
        "{"
        "\"NextState\":\"ON\""
        "}";
        httpd_resp_send(req, json_response, HTTPD_RESP_USE_STRLEN);
    }

    
    
    return ESP_OK;
}

