#include "sensor.h"
#include "light.h"


void app_main(void) {
    data_queue = xQueueCreate(5, sizeof(int));
    if (data_queue != NULL) {
        xTaskCreatePinnedToCore(sensor_task, "Sensor", 2048, NULL, 1, NULL, 1);
        xTaskCreatePinnedToCore(light_task, "Light", 2048, NULL, 1, NULL, 1);
    }
}
