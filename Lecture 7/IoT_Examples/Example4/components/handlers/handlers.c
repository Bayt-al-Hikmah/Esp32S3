#include "handlers.h"





const char* DASHBOARD_HTML =

"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>ESP32 Web Panel</title>"          
"</head>"
"<body>"
    "<h1>Multiple Sensors!</h1>"
    "<div>"
    "The obstacle sensor detected that there is: "
    "<span id =\"obstacle\">"
    "</span>"
    "</div>"
    "<div>"
    "The flame sensor detected that there is: "
    "<span id =\"flame\">"
    "</span>"
    "</div>"
    "<div>"
    "The light sensor value is: "
    "<span id =\"light\">"
    "</span>"
    "</div>"
    "<div>"
    "The moisture sensor value is: "
    "<span id =\"moisture\">"
    "</span>"
    "</div>"
"<script>"
"const source = new EventSource(\"/events\");"

"source.addEventListener('Obstacle Sensor', (e) => {"
  "document.querySelector(\"#obstacle\").innerHTML = e.data;"
"});"

"source.addEventListener('Flame Sensor', (e) => {"
  "document.querySelector(\"#flame\").innerHTML = e.data;"
"});"

"source.addEventListener('Light Sensor', (e) => {"
  "document.querySelector(\"#light\").innerHTML = e.data;"
"});"

"source.addEventListener('Moisture Sensor', (e) => {"
  "document.querySelector(\"#moisture\").innerHTML = e.data;"
"});"


"</script>"
"</body>"
"</html>";


const static char start_sse[] = "HTTP/1.1 200 OK\r\n"
                               "Cache-Control: no-store\r\n"
                               "Content-Type: text/event-stream\r\n"
                               "\r\n"
                               "Event: Start session\r\n"
                               "data: Session Started\r\n"
                               "\r\n";


esp_err_t dashboard_handler(httpd_req_t *req) {
    httpd_resp_send(req, DASHBOARD_HTML, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}


esp_err_t event_handler(httpd_req_t *req){

    sse_session arg;
    arg.mySocketHD = req->handle;
    arg.mySocketFD = httpd_req_to_sockfd(req);

    httpd_socket_send(arg.mySocketHD, arg.mySocketFD, start_sse, strlen(start_sse), 0);

   
    add_sse(arg);
    
    set_obstacle_state(NULL);
    set_fire_state(NULL);
    return ESP_OK;
}

