#include "tasks.h"

void app_main(void){
    xTaskCreatePinnedToCore(blink_task_1, "Task 1", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(blink_task_2, "Task 2", 2048, NULL, 1, NULL, 1);

}
