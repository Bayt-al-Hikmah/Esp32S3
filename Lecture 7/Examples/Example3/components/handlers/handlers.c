#include "utils.h"
#include <string.h>



const char* DASHBOARD_HTML =

"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>ESP32 Web Panel</title>"          
"</head>"
"<body>"
    "<h1>Obstacle Detection!</h1>"
    "<div>"
    "The sensor detected that there is: "
    "<span id =\"obstacle\">"
    "</span>"
    "</div>"
"<script>"
"const source = new EventSource(\"/events\");"

"source.onmessage = (event) => {"
"document.querySelector(\"#obstacle\").innerHTML = event.data;"
"};"

"</script>"
"</body>"
"</html>";


const static char start_sse[] = "HTTP/1.1 200 OK\r\n"
                               "Cache-Control: no-store\r\n"
                               "Content-Type: text/event-stream\r\n"
                               "\r\n"
                               "data: No obstacle\r\n"
                               "\r\n";


esp_err_t dashboard_handler(httpd_req_t *req) {
    httpd_resp_send(req, DASHBOARD_HTML, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}


esp_err_t event_handler(httpd_req_t *req){

    _data.argument.mySocketHD = req->handle;
    _data.argument.mySocketFD = httpd_req_to_sockfd(req);

    httpd_socket_send(_data.argument.mySocketHD, _data.argument.mySocketFD, start_sse, strlen(start_sse), 0);

    return ESP_OK;
}

