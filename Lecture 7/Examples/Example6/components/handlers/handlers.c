#include "handlers.h"

const char* DASHBOARD_HTML =

"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>ESP32 Web Panel</title>"          
"</head>"
"<body>"
    "<h1>Camera Stream!</h1>"
    "<img id=\"cam\" width=\"640\">"
    "<button onclick=\"ledOn()\">LED ON</button>"
    "<button onclick=\"ledOff()\">LED OFF</button>"
    
"<script>"
    "const ws = new WebSocket(\"ws://192.168.4.1/ws\");"

"ws.binaryType = \"blob\";"

"ws.onopen = () => {"
    "requestFrame();"
"};"

"function requestFrame() {"
    "ws.send(\"GET_FRAME\");"
"}"

"let currentUrl = null;"

"ws.onmessage = (event) => {"

    "if (event.data instanceof Blob) {"

        "if (currentUrl) {"
            "URL.revokeObjectURL(currentUrl);"
        "}"

        "currentUrl = URL.createObjectURL(event.data);"

        "document.getElementById(\"cam\").src = currentUrl;"

        "requestFrame();"
    "}"
"};"

"function ledOn() {"
    "ws.send(\"LED_ON\");"
"}"

"function ledOff() {"
    "ws.send(\"LED_OFF\");"
"}"
"</script>"
"</body>"
"</html>";



esp_err_t dashboard_handler(httpd_req_t *req) {
    httpd_resp_send(req, DASHBOARD_HTML, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

