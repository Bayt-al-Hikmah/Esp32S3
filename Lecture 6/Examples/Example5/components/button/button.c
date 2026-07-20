#include "button.h"


static void IRAM_ATTR gpio_isr_handler(void* arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xGuiSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void initialize_handler(){
  gpio_config_t io_conf = {
            .intr_type = GPIO_INTR_NEGEDGE, // Trigger on press (High to Low)
            .mode = GPIO_MODE_INPUT,
            .pin_bit_mask = (1ULL << BUTTON_GPIO),
            .pull_up_en = 1,
        };
        gpio_config(&io_conf);

        
        gpio_install_isr_service(0);
        gpio_isr_handler_add(BUTTON_GPIO, gpio_isr_handler, NULL);
}
