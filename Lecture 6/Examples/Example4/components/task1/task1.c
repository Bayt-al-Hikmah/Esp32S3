#include "task1.h"


void print_task_1(void *pvParameters) {
    while (1) {
        xSemaphoreTake(terminal_mutex, portMAX_DELAY);
	printf("Task 1 is writing a long message...\n");
	vTaskDelay(pdMS_TO_TICKS(100));
	printf("Task 1 finished writing.\n");
	xSemaphoreGive(terminal_mutex);
	vTaskDelay(pdMS_TO_TICKS(500));
   }
}
