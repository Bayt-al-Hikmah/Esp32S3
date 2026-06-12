
#include "tm1637.h"


void pin_init() {
    gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIO, GPIO_MODE_OUTPUT);
    gpio_set_level(CLK, 1);
    gpio_set_level(DIO, 1);
}
void tm1637_start(void) {
    
    gpio_set_level(CLK, 1);
    gpio_set_level(DIO, 1);
    ets_delay_us(2);
    gpio_set_level(DIO, 0);
    ets_delay_us(2);
    gpio_set_level(CLK, 0);
}

void tm1637_stop(void) {
    gpio_set_level(CLK, 0);
    gpio_set_level(DIO, 0);
    ets_delay_us(2);
    gpio_set_level(CLK, 1);
    ets_delay_us(2);
    gpio_set_level(DIO, 1);
}

void tm1637_write_byte(uint8_t data) {
    const uint8_t bits[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    for (int i = 7; i >= 0; i--) {
        gpio_set_level(CLK, 0);
        if (data & bits[i]) {
            gpio_set_level(DIO, 1);
        } else {
            gpio_set_level(DIO, 0);
        }
        ets_delay_us(2);
        gpio_set_level(CLK, 1);
        ets_delay_us(2);
    }

    gpio_set_level(CLK, 0);
    gpio_set_level(DIO, 0);
    ets_delay_us(2);
    gpio_set_level(CLK, 1);
    ets_delay_us(2);
    gpio_set_level(CLK, 0);
}
