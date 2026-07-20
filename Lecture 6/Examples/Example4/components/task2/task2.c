#include "task2.h"


void print_task_2(void *pvParameters) {
    while (1) {
	xSemaphoreTake(terminal_mutex, portMAX_DELAY);
	printf("Task 2 is writing something different...\n");
	vTaskDelay(pdMS_TO_TICKS(100));
	printf("Task 2 finished writing.\n");
	xSemaphoreGive(terminal_mutex);
	vTaskDelay(pdMS_TO_TICKS(300));
    }
}
