#include "esp_wifi.h"  
#include "esp_event.h"  
#include "nvs_flash.h"  
#include "wifi.h"  
#include <string.h>
void wifi_init_ap(void)  {  
	nvs_flash_init();  
	esp_netif_init();  

	esp_event_loop_create_default();  

	esp_netif_create_default_wifi_ap();  

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();  
	esp_wifi_init(&cfg);  
  
	// Configure Access Point settings  
	wifi_config_t ap_config = {  
		.ap = {  
		.ssid = WIFI_SSID,  
		.ssid_len = strlen(WIFI_SSID),  
		.password = WIFI_PASS,  
		.max_connection = 4,  
		.authmode = WIFI_AUTH_WPA_WPA2_PSK  
		},  
	};  
  
	esp_wifi_set_mode(WIFI_MODE_AP);  
  
	esp_wifi_set_config(WIFI_IF_AP, &ap_config);  

	esp_wifi_start();  
}