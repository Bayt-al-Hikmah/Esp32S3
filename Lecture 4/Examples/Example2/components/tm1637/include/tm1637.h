
#ifndef TM1637_H  
#define TM1637_H  

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"

#define CLK 19
#define DIO 21

void pin_init();
void tm1637_start(void);
void tm1637_stop(void);
void tm1637_write_byte(uint8_t data);

#endif 