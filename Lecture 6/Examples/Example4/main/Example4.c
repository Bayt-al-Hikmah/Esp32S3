#include "task1.h"
#include "task2.h"


void app_main(void) {
    terminal_mutex = xSemaphoreCreateMutex();
    if (terminal_mutex != NULL) {
	xTaskCreatePinnedToCore(print_task_1, "Printer1", 2048, NULL,1, NULL, 1);
	xTaskCreatePinnedToCore(print_task_2, "Printer2", 2048, NULL, 1, NULL, 1);
    }
}
