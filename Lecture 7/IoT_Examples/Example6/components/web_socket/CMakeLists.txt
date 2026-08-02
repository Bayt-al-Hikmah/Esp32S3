idf_component_register(SRCS "web_socket.c"
                    INCLUDE_DIRS "include"
REQUIRES esp_http_server  esp_eth esp_driver_gpio espressif__esp32-camera               
)
