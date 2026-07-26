
#ifndef CAMERA_H  
#define CAMERA_H  

#include "esp_camera.h"
#include "esp_http_server.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"  
#include "freertos/task.h"  


#include <esp_event.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_eth.h"
#include "esp_http_server.h"


#include "driver/gpio.h"


#define LED_PIN 21

#define CAM_PIN_PWDN    -1 
#define CAM_PIN_RESET   -1 
#define CAM_PIN_XCLK    15
#define CAM_PIN_SIOD    4
#define CAM_PIN_SIOC    5

#define CAM_PIN_D7      16
#define CAM_PIN_D6      17
#define CAM_PIN_D5      18
#define CAM_PIN_D4      12
#define CAM_PIN_D3      10
#define CAM_PIN_D2      8
#define CAM_PIN_D1       9
#define CAM_PIN_D0       11
#define CAM_PIN_VSYNC   6
#define CAM_PIN_HREF    7
#define CAM_PIN_PCLK    13
#define PART_BOUNDARY "123456789000000000000987654321"

esp_err_t camera_init();

esp_err_t ws_handler(httpd_req_t *req);

#endif