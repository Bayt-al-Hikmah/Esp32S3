#ifndef WIFI_H  
#define WIFI_H  
  
#include <stdint.h>  
#include <string.h>
#include "esp_wifi.h"  
#include "esp_event.h"  
#include "nvs_flash.h"  

#define WIFI_SSID      "ESP32"
#define WIFI_PASS      "Alitigui123"

void wifi_init_ap(void);  
  
#endif