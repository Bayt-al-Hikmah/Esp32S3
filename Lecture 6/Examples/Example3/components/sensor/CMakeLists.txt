idf_component_register(SRCS "sensor.c"

INCLUDE_DIRS "include"
REQUIRES esp_driver_gpio esp_adc queue
)
