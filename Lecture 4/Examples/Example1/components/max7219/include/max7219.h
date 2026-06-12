#ifndef MAX7219_H  
#define MAX7219_H  

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"

#define DIN 21
#define CLK 19
#define CS  20

void  max7219_init();
void max7219_send(uint8_t reg, uint8_t data);

#endif