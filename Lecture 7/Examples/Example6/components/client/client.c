#include "client.h"


void fetch_weather_data(void) {
    esp_http_client_config_t config = {
        .url = "https://power.larc.nasa.gov/api/temporal/daily/point?parameters=T2M&community=RE&longitude=3.0588&latitude=36.7538&start=20260510&end=20260511&format=JSON",
        .event_handler = client_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_method(client, HTTP_METHOD_GET);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        printf("HTTP GET Status = %d, content_length = %lld\n",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        printf("HTTP GET request failed: %s\n", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}
