#include "leds.h"


bool redLed = false;
bool yellowLed = false;
bool greenLed = false;


int toggle_led(int ledNumber){
    if (ledNumber == 1) {
        greenLed ^= 1;
        gpio_set_level(10, greenLed);
        return greenLed;
    } else if (ledNumber == 2) {
        redLed ^= 1;
        gpio_set_level(11, redLed);
        return redLed;
    } else if (ledNumber == 3) {
        yellowLed ^= 1;
        gpio_set_level(12, yellowLed );
        return yellowLed;
    }
    return false;
}

void initialize_pins(){
    gpio_set_direction(10, GPIO_MODE_OUTPUT);
    gpio_set_direction(11, GPIO_MODE_OUTPUT);
    gpio_set_direction(12, GPIO_MODE_OUTPUT);
}