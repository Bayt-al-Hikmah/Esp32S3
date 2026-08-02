#ifndef WEBSOCKET_H
#define WEBSOCKET_H


#include "esp_http_server.h"
#include <esp_event.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_eth.h"
#include "driver/gpio.h"
#include "esp_camera.h"


#define LED_PIN 21


esp_err_t ws_handler(httpd_req_t *req);


#endif
