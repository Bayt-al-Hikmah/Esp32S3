#ifndef WIFI_H  
#define WIFI_H  
  
#include <stdint.h>  
#include "esp_wifi.h"  
#include "esp_event.h"  
#include "nvs_flash.h"  
#include <string.h>

#define WIFI_SSID      "ESP32_Ali"
#define WIFI_PASS      "Alitigui123"

void wifi_init_ap(void);  
  
#endif