#include "tasks.h"

TaskHandle_t myTaskHandle = NULL;

void app_main(void) {
    xTaskCreatePinnedToCore(blink_task_1, "Task 1", 2048, NULL, 1, &myTaskHandle, 1);
    vTaskDelay(pdMS_TO_TICKS(30000));

    vTaskSuspend(myTaskHandle);
    vTaskDelay(pdMS_TO_TICKS(30000));

    vTaskResume(myTaskHandle);
    vTaskDelay(pdMS_TO_TICKS(10000));

    vTaskDelete(myTaskHandle);
}
