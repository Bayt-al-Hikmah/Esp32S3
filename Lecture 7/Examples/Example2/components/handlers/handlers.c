#include "esp_http_server.h"
#include "servo.h"
#include "utils.h"

const char* DASHBOARD_HTML =

"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>ESP32 Web Panel</title>"          
"</head>"
"<body>"
    "<h1>Control LEDs!</h1>"
    "<div>"
    "<span>"
    "<input type=\"range\" id=\"angle\" min=\"0\" max=\"180\" value=\"0\"/>"
    "<label for=\"angle\">Angle : 0</label>"
    "</span>"
    "</div>"
"<script>"
"document.querySelector(\"#angle\").addEventListener(\"change\", (e) => {"
        "fetch(\"/servo\", {"
            "method: \"POST\","
            "headers: {"
                "\"Content-Type\": \"application/json\","
            "},"
            "body: JSON.stringify({ \"Angle\":e.target.value})})"
        ".then(response => response.json())"
        ".then(data => {e.target.nextElementSibling.innerHTML=\"Angle :\" + e.target.value});"
    "});"
"</script>"
"<style>"
"div{"
"display:flex;"
"row-gap:10px;"
"align-items:center;"  
"flex-direction:column;"
"}"
"</style>"
"</body>"
"</html>";


esp_err_t dashboard_handler(httpd_req_t *req) {
    httpd_resp_send(req, DASHBOARD_HTML, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}


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

