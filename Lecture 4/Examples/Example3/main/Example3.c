#include "lcd.h"  
  
void app_main(void) {
    initialize_lcd();
    write_word("Hello world");
}