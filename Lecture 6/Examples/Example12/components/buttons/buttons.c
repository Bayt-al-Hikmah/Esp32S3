#include "buttons.h"

void count_up(void *arg){

    gpio_reset_pin(4);
    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_set_pull_mode(4,GPIO_PULLDOWN_ONLY);
    
    while (1){
	    if(gpio_get_level(4)){
			int32_t* val = (int32_t*)arg;
			*((int32_t*)arg)= *val + 1;
	        set_counter(*((int32_t*)arg));
	        printf("counter  is now %ld.\n", *((int32_t*)arg));
            vTaskDelay(pdMS_TO_TICKS(500)); 
        }
		vTaskDelay(pdMS_TO_TICKS(10));
    } 
} 

void reset(void *arg){
    gpio_reset_pin(5);
    gpio_set_direction(5, GPIO_MODE_INPUT);
    gpio_set_pull_mode(5,GPIO_PULLDOWN_ONLY);
    while (1){
	    if(gpio_get_level(5)){
	        *((int32_t*)arg) = 0;
	        set_counter(*((int32_t*)arg));
	        printf("counter  reset to 0 \n count =  %ld.\n", *((int32_t*)arg));
	        vTaskDelay(pdMS_TO_TICKS(500));
	    }
	    vTaskDelay(10 / portTICK_PERIOD_MS);
    }
	
}
