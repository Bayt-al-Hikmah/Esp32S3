
#include "driver/ledc.h"

void set_angle(int angle){
    int pwm = 0;
    pwm = 4095.0/20.0 * ( (float)angle * 1/90  + 0.5);
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, (int)pwm);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}