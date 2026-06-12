#include "lcd.h"

void lcd_pulse_enable() {
    gpio_set_level(E, 1);
    ets_delay_us(1);
    gpio_set_level(E, 0);
    ets_delay_us(20);
}

void lcd_send_bits(uint8_t bits){
	gpio_set_level(D4, bits & 0b1);
	gpio_set_level(D5, bits & 0b10);
	gpio_set_level(D6, bits & 0b100);
	gpio_set_level(D7, bits & 0b1000);
	lcd_pulse_enable();
}

void send_command(uint8_t command){
	lcd_send_bits(command >> 4);
	lcd_send_bits(command );
}

void lcd_gpio_init(){

    gpio_set_direction(D4, GPIO_MODE_OUTPUT);
    gpio_set_direction(D5, GPIO_MODE_OUTPUT);
    gpio_set_direction(D6, GPIO_MODE_OUTPUT);
    gpio_set_direction(D7, GPIO_MODE_OUTPUT);
    gpio_set_direction(RS, GPIO_MODE_OUTPUT);
    gpio_set_direction(E, GPIO_MODE_OUTPUT);


    gpio_set_level(RS, 0);
    gpio_set_level(E, 0);
}


void initialize_lcd(){ 
   lcd_gpio_init();

   vTaskDelay(pdMS_TO_TICKS(50)); 

   lcd_send_bits(0x03); 
   vTaskDelay(pdMS_TO_TICKS(5)); 

   lcd_pulse_enable(); 
   ets_delay_us(100);

   lcd_pulse_enable(); 
   ets_delay_us(10); 


   lcd_send_bits(0x02);

   send_command(0x28); 
   send_command(0x0C); 
   send_command(0x01); 

   vTaskDelay(pdMS_TO_TICKS(2)); 
   
}

void clear_display(){
	send_command(0x01);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void move_to(uint8_t line, uint8_t column){
	if (line == 1){
	send_command(0x80 | column);
	}else{
		send_command(0xC0 | column);
	}
}

void write_character(char c){
  gpio_set_level(RS, 1);
  send_command(c);
  gpio_set_level(RS, 0);
}

void write_word(char message[]){
  for (int i=0;message[i]!='\0';i++){
    write_character(message[i]);
  }
}