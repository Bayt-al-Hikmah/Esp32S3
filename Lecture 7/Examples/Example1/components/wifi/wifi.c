#include "wifi.h"

void wifi_init_ap(void)  {

	nvs_flash_init();
	esp_netif_init();

	esp_event_loop_create_default();

	esp_netif_create_default_wifi_ap();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);

	wifi_config_t ap_config = {
		.ap = {
		.ssid = "ESP32_AP",
		.ssid_len = strlen("ESP32_AP"),
		.password = "mypassword123",
		.max_connection = 4,
		.authmode = WIFI_AUTH_WPA_WPA2_PSK
		},
	};

	esp_wifi_set_mode(WIFI_MODE_AP);
	esp_wifi_set_config(WIFI_IF_AP, &ap_config);
	esp_wifi_start();
}
