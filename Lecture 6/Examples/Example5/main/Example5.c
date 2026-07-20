#include "led.h"
#include "button.h"

void app_main(void) {

    xGuiSemaphore = xSemaphoreCreateBinary();
    if (xGuiSemaphore != NULL) { 
        initialize_handler();
        xTaskCreatePinnedToCore(led_toggle_task, "LED_Task", 2048, NULL, 10, NULL, 1);
    }
}
