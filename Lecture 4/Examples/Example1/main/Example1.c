
#include "max7219.h"


const uint8_t  heart[8] = {
    0B00000000,
    0B01100110,
    0B11111111,
    0B11111111,
    0B11111111,
    0B01111110,
    0B00111100,
    0B00011000
  };


void app_main(void) {

    max7219_init();
    while (1) {
        for (int i = 0; i < 8; i++) {
            max7219_send(i + 1, heart[i]);
        }
        
        vTaskDelay(pdMS_TO_TICKS(500));

        for (int i = 0; i < 8; i++) {
           max7219_send(i + 1, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}