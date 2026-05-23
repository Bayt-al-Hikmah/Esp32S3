#include "driver/gpio.h"
#include <stdbool.h>


bool redLed = false;
bool yellowLed = false;
bool greenLed = false;


bool toggle_led(int ledNumber){
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