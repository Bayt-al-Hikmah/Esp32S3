#include "tm1637.h"

const uint8_t segment_map[] = {
    0x3F, 
    0x06, 
    0x5B, 
    0x4F, 
    0x66, 
    0x6D, 
    0x7D, 
    0x07, 
    0x7F, 
    0x6F  
};

void app_main(void) {
    pin_init() ;

    tm1637_start();
    tm1637_write_byte(0x40); 
    tm1637_stop();
  
    tm1637_start();
    tm1637_write_byte(0x8F); 
    tm1637_stop();

    while (1) {
        
        tm1637_start();
        tm1637_write_byte(0xC0); 
        tm1637_write_byte(segment_map[1]); 
        tm1637_write_byte(segment_map[2]); 
        tm1637_write_byte(segment_map[3]); 
        tm1637_write_byte(segment_map[4]); 
        tm1637_stop();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}