#ifndef LCD_H  
#define LCD_H  

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
  
#define D4 1
#define D5 2
#define D6 42
#define D7 41
#define RS 20
#define E 19


void initialize_lcd(void);  
  
void clear_display(void);  
void move_to(uint8_t line, uint8_t column);  
  
void write_character(char c);  
void write_word(char message[]);  
  
#endif