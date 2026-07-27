#ifndef EVENTS_H
#define EVENTS_H

#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "esp_wifi.h"

esp_err_t client_event_handler(esp_http_client_event_t *evt);
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) ;



#endif
