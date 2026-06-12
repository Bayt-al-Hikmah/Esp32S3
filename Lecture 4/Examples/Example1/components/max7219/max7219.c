#include "max7219.h"


void send_byte(uint8_t data){
   const uint8_t bits[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    for (int i = 0; i < 8; i++) {
        gpio_set_level(CLK, 0);
        if ((data & bits[i]) != 0){
            gpio_set_level(DIN, 1);
        }
        else{
            gpio_set_level(DIN, 0);
        }    
        ets_delay_us(2);
        gpio_set_level(CLK, 1);
        ets_delay_us(2);
    }
}


void max7219_send(uint8_t reg, uint8_t data){
    gpio_set_level(CS, 0);
    send_byte(reg);
    send_byte(data);
    gpio_set_level(CS, 1);
}

void max7219_init(){
    gpio_set_direction(DIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(CS, GPIO_MODE_OUTPUT);

    gpio_set_level(CS, 1);
    max7219_send(0x0F, 0x00);
    max7219_send(0x0C, 0x01);
    max7219_send(0x0B, 0x07);
    max7219_send(0x0A, 0x08);
    max7219_send(0x09, 0x00);
  
    for (int i = 1; i <= 8; i++){
        max7219_send(i, 0x00);
    }
}
