idf_component_register(SRCS "camera.c"  
INCLUDE_DIRS "include"

REQUIRES esp_http_server espressif__esp32-camera  esp_driver_gpio
)