#include <stdio.h>
#include "static.h"


esp_err_t serve_file(httpd_req_t *req, char* file_name){
    char buffer[255];
    FILE *fptr;

    fptr = fopen(file_name, "r");
    if (fptr == NULL) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File does not exist");
        return ESP_OK;
    }
    
    int length = fread(buffer, sizeof(char), 255, fptr);
    while( length>= 255){
        httpd_resp_sendstr_chunk(req, buffer);
        length=fread(buffer, sizeof(char), 255, fptr);
    }
    buffer[length]='\0';
    httpd_resp_sendstr_chunk(req, buffer);
    httpd_resp_sendstr_chunk(req, NULL);

    return ESP_OK;
}


esp_err_t dashboard_handler(httpd_req_t *req) {
    
    return serve_file(req,"/web/index.html");
}

esp_err_t style_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/css");
    return serve_file(req,"/web/style.css");
}
esp_err_t script_handler(httpd_req_t *req) {
    
    httpd_resp_set_type(req, "text/javascript");
    return serve_file(req,"/web/script.js");
}