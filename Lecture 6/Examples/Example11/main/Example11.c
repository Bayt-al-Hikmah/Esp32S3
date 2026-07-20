#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void memory_monitor_task(void *pvParameters) {
    int local_array[100]; 
    while (1) {
        UBaseType_t high_water_mark = uxTaskGetStackHighWaterMark(NULL);
        printf("Task Stack High Water Mark: %u\n", high_water_mark);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void) {
    xTaskCreatePinnedToCore(memory_monitor_task, "MemTask", 2048, NULL, 1, NULL, 1);
}
