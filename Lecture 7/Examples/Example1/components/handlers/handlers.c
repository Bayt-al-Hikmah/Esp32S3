#include "esp_http_server.h"
#include "handlers.h"
#include "driver/gpio.h"
#include <stdbool.h>



bool redLed = false;
bool yellowLed = false;
bool greenLed = false;
const char* DASHBOARD_HTML =

"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>ESP32 Web Panel</title>"          
"</head>"
"<body>"
    "<h1>Control LEDs!</h1>"
    "<div>"
    "<button class='green'>Turn Led 1 ON</button>"
    "<button class='green'>Turn Led 2 ON</button>"
    "<button class='green'>Turn Led 3 ON</button>"
    "</div>"
"<script>"
"document.querySelectorAll(\"button\").forEach((button, index) => {"
    "button.addEventListener(\"click\", (e) => {"
        "fetch(\"/led\", {"
            "method: \"POST\","
            "headers: {"
                "\"Content-Type\": \"application/json\","
            "},"
            "body: JSON.stringify({ \"Led\":index + 1})})"
        ".then(response => response.json())"
        ".then(data => {e.target.innerHTML = \"Turn Led \" + (index + 1) + \" \" + data.NextState; e.target.className= data.NextState=='ON'?'green':'red'});"
    "});"
"});"
"</script>"
"<style>"
"div{"
"display:flex;"
"row-gap:10px;"
"align-items:center;"  
"flex-direction:column;"
"}"
".red{"
    "background:red;"
"}"
".green{"
    "background:green;"
"}"
"button{"
"border:none;"
"color:white;"
"padding:10px;"
"}"
"</style>"
"</body>"
"</html>";



bool toggle_led(int ledNumber){
    if (ledNumber == 1) {
        greenLed ^= 1;
        gpio_set_level(10, greenLed);
        return greenLed;
    } else if (ledNumber == 2) {
        redLed ^= 1;
        gpio_set_level(11, redLed);
        return redLed;
    } else if (ledNumber == 3) {
        yellowLed ^= 1;
        gpio_set_level(12, yellowLed );
        return yellowLed;
    }
    return false;
}


int extract_index(char* buffer, char* argument, int size){
    bool match = false;
    for (int i = 0; buffer[i] != '\0'; i++){
        if(buffer[i] == argument[0]){
            match = true;
            for (int j = 1; buffer[j] != '\0' && j < size; j++){
                if(buffer[i+j] != argument[j]){
                    match = false;
                    break ;
                }
            }
            if (match){
                return i + size;
            }
        }
    }
    return -1;
}

esp_err_t dashboard_handler(httpd_req_t *req) {
    httpd_resp_send(req, DASHBOARD_HTML, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}


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

