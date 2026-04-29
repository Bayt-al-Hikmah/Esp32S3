## Objectives
- Understand General Purpose Input/Output (GPIO) pins
- Working with Motors

## General Purpose Input/Output (GPIO) Pins
General Purpose Input/Output (GPIO) pins allow the ESP32-S3 to connect and communicate with the external world. These pins can be configured either as inputs or outputs, depending on the needs of the project.

The ESP32-S3 features a highly flexible GPIO matrix, providing up to 45 physical GPIO pins (with the exact number accessible depending on our specific DevKit). These pins are divided into two main functional uses
- **Digital pins:** Used to read or write digital signals HIGH (1) or LOW (0).
- **Analog pins:** Used to read analog signals, capturing a range of voltage values through internal ADCs (Analog-to-Digital Converters).


<img src="./attachments/Esp32_Gpio.png"/>

### Digital GPIO Pins
Unlike simpler microcontrollers, the ESP32-S3 allows almost any pin to be configured for digital input or output functions. These pins handle only two states: HIGH (3.3V) and LOW (0V). This means they can either send or receive digital signals, such as turning an LED on or off, or reading the state of a button.

Each digital pin can be configured as either an input or an output, depending on the requirements of the circuit. When set as an input, the pin reads signals from external components. When set as an output, it sends signals to control devices.
#### Digital GPIO Functions in ESP-IDF
When programming in C using the ESP-IDF framework, we use specific driver functions to control digital pins. To access these, we must include the `#include "driver/gpio.h"` header.

**`gpio_set_direction()`:** This function is used to configure the mode of a digital pin. It allows us to set the pin as either an input or an output, It accepts two arguments:
- The pin number (using the `GPIO_NUM_x` macro, e.g., `GPIO_NUM_13`)
- The **mode** we want to set (`GPIO_MODE_INPUT` or `GPIO_MODE_OUTPUT`)

**`gpio_set_pull_mode()`**: This function used to set the internal resistor so we avoid unstable readings (like when using push buttons), it take two argument the pin number and a state like `GPIO_PULLUP_ONLY` or `GPIO_PULLDOWN_ONLY`.

**`gpio_set_level()`:** This function is used to send a digital signal to a pin. It allows us to set the pin to HIGH or LOW, it accepts two arguments:
- The pin number
- The signal state (`1` for HIGH or `0` for LOW)

**`gpio_get_level()`:** Finally this  function is used to read the state of a digital pin. It returns either `1` (HIGH) or `0` (LOW) depending on the input signal, It accepts one argument:
- The pin number


### Digital Output and Input Devices
Now we know the basic functions to deal with the Digital GPIO pins lets explore some of common and most used digital sensor and devices 
#### LEDs and Push Buttons
LEDs, or light-emitting diodes, are digital output components that emit light when an electric current passes through them. They are polarized, which means they allow current to flow in only one direction. LEDs have two states: ON and OFF. We can turn an LED ON by sending a HIGH signal from a digital pin, and turn it OFF by sending a LOW signal. LEDs are often used as indicators to show the status of a system.

Push buttons are used as input devices. They also have two states: pressed and released. When a button is pressed, it sends a signal (HIGH or LOW, depending on the circuit design) to the ESP32-S3. The microcontroller reads this signal and performs an action based on the button state.

Let’s build a simple circuit where we turn an LED on and off using a push button. When we press the push button, the LED turns on, and when we press it again, it turns off.

For this project, we will use GPIO 4 to control the LED and GPIO 1 for the push button. We will use a 220-ohm resistor with the LED, and for the push button, we will configure the GPIO pin with a pull-down resistor.

<img src="./attachments/circuit1.png" />

Now, Lets create our program, we start by including the necessary libraries. After that, we declare a variable to keep track of the LED’s state.

In the `app_main` function, we first reset the pins. Then, we configure GPIO 2 as an output for the LED and GPIO 4 as an input for the push button, We also configure GPIO 4 to use a pull-down resistor, so its default state is LOW (0). This ensures that the pin reads a stable 0 when the button is not pressed.    
After that, we create an infinite loop where we continuously read the state of GPIO 4. If the pin is HIGH, it means the user has pressed the button. We then toggle the `LedState` variable and update the LED accordingly.

Finally, we add a 500 ms delay to prevent the button bouncing effect, ensuring that a single press is not detected multiple times. 
```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdbool.h>

bool LedState = false;

void app_main(void){

    gpio_reset_pin(4);
    gpio_reset_pin(2);
    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_set_pull_mode(4,GPIO_PULLDOWN_ONLY);

    while (1) {
        if(gpio_get_level(4)){
            LedState ^=1;
            gpio_set_level(2, LedState);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }  
    }
}
```
#### Buzzers
A buzzer is a special component that produces sound when we send a HIGH signal to it. It is commonly used as an alert or notification device in electronic projects, such as alarms, timers, and warning systems. There are two main types of buzzers:
- **Active buzzer:** This type has a built-in oscillator that generates sound at a predefined frequency. We only need to send a HIGH or LOW signal to turn it ON or OFF. It is simple to use but offers limited control over the sound.
- **Passive buzzer:** This type does not have a built-in oscillator. Instead, it allows us to generate sound at different frequencies by sending PWM (Pulse Width Modulation) signals from the ESP32-S3. This gives us more control over the tone, which makes it possible to play melodies or different alert sounds.

Let’s build a simple project where an LED blinks and an active buzzer produces sound when a push button is pressed. We will connect the LED to GPIO 2, the buzzer to GPIO 20, and the push button to GPIO 4.

<img src="./attachments/circuit2.png" />

Now, let’s create our C program using ESP-IDF, We start by including the necessary libraries. For this example, we don’t need to keep track of any state, so we go directly to the `app_main` function. First, we reset the pins we will use, then configure GPIO 4 as an input and GPIO 2 and GPIO 20 as outputs. Next, we set GPIO 4 to pull-down mode so that it reads LOW (0) by default when the button is not pressed.

Finally, we create an infinite loop. Inside this loop, we use a nested loop that runs while the push button is pressed (when GPIO 4 is HIGH). When this happens, we set GPIO 20 HIGH to activate the buzzer, and we also blink the LED connected to GPIO 2 by turning it ON, waiting 500 ms, turning it OFF, and waiting another 500 ms. This creates a blinking effect.

Outside the inner loop, we set GPIO 20 to LOW, which deactivates the buzzer when the push button is not pressed.
```C
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main(void){
    gpio_reset_pin(4);
    gpio_reset_pin(2);
    gpio_reset_pin(20);
    
    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    gpio_set_direction(20, GPIO_MODE_OUTPUT);
    gpio_set_direction(4, GPIO_MODE_INPUT);
    
    gpio_set_pull_mode(4,GPIO_PULLDOWN_ONLY);
    while (1) {
        while(gpio_get_level(4)){
            gpio_set_level(20, 1);
            gpio_set_level(2, 1);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            gpio_set_level(2, 0);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }  
         gpio_set_level(20, 0);
    }
}
```
#### Obstacle Sensor
Another important sensor is the obstacle sensor. It allows us to detect the presence of obstacles using infrared (IR) signals. The sensor has an LED that emits infrared light and a receiver that detects the reflected IR signals. When an object is in front of the sensor, the infrared light reflects back to the receiver, allowing the sensor to detect the obstacle.

The sensor also includes an internal variable resistor (potentiometer) that allows us to adjust the detection distance. The obstacle sensor has three pins: VCC, GND, and OUT (output). The output pin becomes LOW (0) when an obstacle is detected and HIGH (1) when no obstacle is present.

We use the built-in potentiometer to adjust the detection distance of the obstacle sensor.

When the potentiometer is turned clockwise, the detection distance becomes larger, making the sensor more sensitive. When it is turned counterclockwise, the detection distance becomes shorter, making the sensor less sensitive.

<img src="./attachments/obstacle.png" />

Let’s create a simple project where a buzzer produces a sound when an obstacle sensor detects an object. We will use GPIO 20 to control the buzzer and GPIO 4 to read the signal from the obstacle sensor.    

<img src="./attachments/circuit3.png" />

Now let’s create the program. As before, we start by including the libraries our program needs. In the `app_main` function, we reset GPIO pins 4 and 20. Then, we configure GPIO 4 as an input and GPIO 20 as an output. We also set GPIO 4 to pull-down mode.

Next, we create an infinite loop where the program continuously runs. Inside this loop, we check the state of GPIO 4. If it is LOW (0), it means an object is detected, so we set GPIO 20 to HIGH (1) to turn the buzzer on. Otherwise, we set GPIO 20 to LOW (0) to turn the buzzer off.

```c
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void){

    gpio_reset_pin(4);
    gpio_reset_pin(20);

    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_set_direction(20, GPIO_MODE_OUTPUT);

    gpio_set_pull_mode(4,GPIO_PULLDOWN_ONLY);

    while (1) {
        if(gpio_get_level(4) == 0){
            gpio_set_level(20, 1);

        } else{
            gpio_set_level(20, 0);

        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
```
#### Relays
The ESP32-S3 digital pins operate strictly on 3.3V logic and can supply a maximum current of about 40 mA per pin. This is enough to control small electronic components such as LEDs, buzzers, and logic inputs to other modules.

However, when we want to build larger projects, we may need to control devices that require higher voltages (like 12V DC or 220V AC) or higher currents, such as lamps, fans, pumps, or motors. The ESP32-S3 pins cannot supply power for these devices directly, and connecting them without protection will permanently destroy the microcontroller.

To solve this problem, we use relays. A relay is an electrically controlled switch that allows a low-power, 3.3V signal from the ESP32-S3 to control high-power devices safely. Inside the relay, there is a coil that plays a key role. When the ESP32-S3 sends a control signal, a small current flows through the coil, which produces a magnetic field. This magnetic field pulls a metal armature, causing the internal switch to close and allowing current to flow in the high-power circuit. When the microcontroller stops sending the signal, the current in the coil stops, the magnetic field disappears, and a spring pushes the armature back to its original position, opening the switch again.

<img src="./attachments/relay.png" />

Let’s build a simple project using a relay and an object sensor. In this project, we will use the obstacle sensor to turn a light ON when the user passes their hand in front of the sensor, and turn it OFF when the user passes their hand again.

First, we connect the obstacle sensor:
- **VCC → 3.3V** on the DevKit
- **GND → GND** on the DevKit
- **Out pin → GPIO 10**

Next, we connect the relay module, Make sure your relay module can be triggered by a 3.3V logic signal, which most standard optical-isolated relay modules can. Control Side, Low Voltage:

- **VCC → 3.3V** 
- **GND → GND**
- **IN (Control Pin) → GPIO 1**

**High-Power Side:** On the high-power side, the relay has three terminals: COM (Common), NO (Normally Open), and NC (Normally Closed). To control a lamp:

- Connect the **live (phase) wire** from the power source to COM.
- Connect the NO terminal to one wire of the lamp.    
- Connect the other wire of the lamp back to the neutral.


<img src="./attachments/circuit4.png" />


Now let’s create the C program for our project. 
```c
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

bool lightState = false;

void app_main(void){

    gpio_reset_pin(1);
    gpio_reset_pin(10);
    

    gpio_set_direction(1, GPIO_MODE_OUTPUT); 
    gpio_set_direction(10, GPIO_MODE_INPUT);  

    gpio_set_pull_mode(10,GPIO_PULLDOWN_ONLY);

    while (1) {
        if(gpio_get_level(10) == 0){
            lightState ^=1;
            gpio_set_level(1,lightState);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        } 
        
    }
}
```
#### Ultrasonic Sensor
Ultrasonic sensors are one of the most useful sensors used with microcontrollers. They help us measure distance by using sound waves. When we want to calculate the depth of a well, or how long a cave is, we often rely on sound. For example, in the past, people would throw a rock into a well and listen for the sound when it hit the ground. By measuring the time it took for the sound to return, they could estimate how deep the well was.

The same idea applies when exploring a cave. An adventurer might shout and listen for the echo. The time it takes for the echo to come back gives an idea of how far away the cave walls are.

In a similar way, an ultrasonic sensor such as the HC-SR04 sends out high-frequency sound waves and measures how long it takes for the echo to return. Using this time, the ESP32-S3 calculates the distance to an object.

<img src="./attachments/ultrasonic.png"/>

The HC-SR04 ultrasonic sensor has four pins: VCC and GND are used to power the sensor. The TRIG pin is used to send a short HIGH pulse (about 10 microseconds), which makes the sensor emit an ultrasonic sound wave. The ECHO pin then goes HIGH and stays HIGH until the reflected sound wave returns to the sensor. The duration of this HIGH signal represents the time taken for the sound to travel to an object and back.

Using this, we can calculate the distance by multiplying the duration by the speed of sound, then dividing by two. We divide by two because the measured duration represents the time it takes for the sound wave to travel to the object and back, which is twice the actual distance.

Let’s create a project where we use three LEDs to indicate distance:
- The red LED turns on when the object is 60 cm or more away.
- The yellow LED turns on when the object is at least 30 cm away.
- The green LED turns on when the object is closer than 30 cm.

Each LED is connected to a GPIO pin through a 220 Ω resistor:
- **GPIO 13** → Red LED
- **GPIO 12** → Yellow LED
- **GPIO 11** → Green LED


The HC-SR04 Ultrasonic Sensor has four pins:
1. **VCC** Connect to the 5V pin .
2. **GND** Connect to a GND pin on the DevKit
3. **Trig (Trigger)** Used to send the ultrasonic pulse, we connect it to GPIO 20
4. **Echo** Used to receive the reflected signal, we connect it to GPIO 19

The Echo pin outputs 5V, which may damage microcontrollers that operate at 3.3V logic levels.   
To safely connect it, use a voltage divider:
- One 220 Ω resistor
- One 330 Ω resistor

<img src="./attachments/circuit5.png" />

After finishing the circuit, it is time to write the program. We will use the `esp_timer_get_time()` function to measure microseconds.

```c
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_rom_sys.h" 

void app_main(void) {
    gpio_set_direction(13, GPIO_MODE_OUTPUT);
    gpio_set_direction(12, GPIO_MODE_OUTPUT);
    gpio_set_direction(11, GPIO_MODE_OUTPUT);
    gpio_set_direction(20, GPIO_MODE_OUTPUT);
    gpio_set_direction(19, GPIO_MODE_INPUT);

    float duration, distance;

    while (1) {
        
        gpio_set_level(20, 0);
        esp_rom_delay_us(2);
        gpio_set_level(20, 1);
        esp_rom_delay_us(10);
        gpio_set_level(20, 0);

        uint32_t start_time = 0;
        uint32_t end_time = 0;

        while(gpio_get_level(19) == 0) {
            start_time = esp_timer_get_time();
        }
        
        while(gpio_get_level(19) == 1) {
            end_time = esp_timer_get_time();
        }

        duration = (float)(end_time - start_time);
        distance = (duration * 0.0343) / 2.0;

        if (distance >= 60) {  
            gpio_set_level(13, 1); 
            gpio_set_level(12, 0); 
            gpio_set_level(11, 0);
        } else if (distance >= 30) {  
            gpio_set_level(13, 0); 
            gpio_set_level(12, 1); 
            gpio_set_level(11, 0); 
        } else {  
            gpio_set_level(13, 0); 
            gpio_set_level(12, 0);  
            gpio_set_level(11, 1); 
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```
We start by including the necessary libraries: one for controlling GPIO pins, and others for handling tasks and timing functions. Inside the `app_main` function, we configure the GPIO pins (11, 12, 13, 20) as outputs  (for LEDs and the trigger pin) and GPIO 19 as an input (for the echo pin). We also declare two variables to store the signal duration and the calculated distance.

Next, we enter an infinite loop. We begin by sending a short pulse from GPIO 20: first setting it LOW, then HIGH for 10 microseconds, and then LOW again. This pulse triggers the HC-SR04 ultrasonic sensor to emit a sound wave. After that, we measure the time it takes for the echo signal to return. The `start_time` is recorded when the echo pin goes HIGH, and the `end_time` is recorded when it goes LOW again, indicating the sound wave has returned to the sensor.

Using this time difference, we calculate the duration of the echo pulse and then compute the distance based on the speed of sound. Finally, we use conditional statements to control three LEDs: one turns on if the object is far (≥ 60 cm), another for a medium distance (≥ 30 cm), and the last one for a short distance. A short delay is added at the end of each loop iteration before repeating the process.

### Pulse Width Modulation
Pulse Width Modulation (PWM) is a technique used to simulate an analog output using a digital signal, The principle of PWM is based on switching the output signal ON and OFF repeatedly during a constant time period T.   

This fixed period T is called the PWM period, and within this period, the signal stays ON for a certain amount of time and OFF for the rest of the time, the percentage of time that the signal remains ON during one period is called the duty cycle.  

The output voltage is determined by the average value of the signal over one complete period. For example:
- If the output stays ON for the entire period (100% duty cycle), the average voltage will be **3.3V**.
- If the output never turns ON (0% duty cycle), the average voltage will be **0V**.
- If the output stays ON for half of the period (50% duty cycle), the average voltage will be **1.65V**.


```math
V_{average}​=\frac{Ton}{T}​​×Vmax​=DutyCycle×Vmax​
```

<img src="./attachments/PWM.png" />

The ESP32-S3 features a highly flexible GPIO matrix. Unlike older microcontrollers that restrict PWM to specific hardware pins, the ESP32-S3 allows us to route PWM signals to all pins that can act as outputs using the LED Control (LEDC) peripheral. 

To work with PWM in ESP-IDF, we use the LEDC (LED Controller) peripheral. This hardware module generates PWM signals efficiently without heavy CPU usage. The configuration is done in two main steps: first, we set up a timer that defines the PWM signal’s frequency and resolution, and then we configure a channel that connects this timer to a specific GPIO pin. Once configured, we can control the signal by adjusting the duty cycle, which determines how long the signal stays HIGH during each PWM period.

```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

void app_main() {

    // 1. Configure the LEDC Timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_12_BIT, // 12-bit resolution
        .freq_hz          = 5000, // 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // 2. Configure the LEDC Channel
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .gpio_num       = 2,
        .duty           = 0,   // start with 0% duty
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    // 3. Change duty cycle in runtime
    while (1) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 2047); // ~50%
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0); // 0%
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```
The timer configuration is responsible for defining how the PWM signal behaves. The `freq_hz` field sets how fast the signal repeats; in this example, 5000 Hz means the signal completes 5000 cycles per second. The `duty_resolution` determines how many discrete steps are available for the duty cycle. With a 12-bit resolution, the counter ranges from 0 to 4095, giving fine control over the signal. These two parameters are linked: increasing frequency reduces the maximum achievable resolution, and increasing resolution limits the maximum frequency. The clock source (`clk_cfg`) is usually set to automatic selection, allowing the driver to choose the most suitable clock.

The channel configuration connects the timer to a physical GPIO pin. The `channel` field selects one of the available LEDC channels, while `timer_sel` links that channel to the previously configured timer. The `gpio_num` determines which pin outputs the PWM signal. The `duty` field sets the initial duty cycle, and `hpoint` defines when the signal goes HIGH within the PWM period (typically left at 0 for standard behavior). On ESP32-S3, all channels operate in low-speed mode, and changes to PWM parameters must be explicitly applied by software.

Once the PWM is running, we control it by modifying the duty cycle. The duty cycle represents the proportion of time the signal is HIGH within one period. With a 12-bit resolution, valid values range from 0 to 4095. A value of 0 corresponds to 0% (always OFF), while 4095 corresponds to nearly 100% (always ON). For example, a value of 2047 produces approximately a 50% duty cycle. To apply a new duty value, we first call `ledc_set_duty()` to update the internal value, and then `ledc_update_duty()` to push the change to the hardware. Without the update call, the new duty will not take effect. By continuously adjusting the duty cycle, we can control brightness, motor speed, or any other PWM-driven behavior.
### Analogue GPIO Pins
We saw digital signals and how they handle only two values: HIGH and LOW (0 V and 3.3 V), we also saw how we can use PWM (Pulse Width Modulation) to simulate an analog output. However, when working with electronic devices and sensors, we often need to read analog signals, not just 0 V or 3.3 V, but any value in between.

This is where the Analog-to-Digital Converter (ADC) comes in. An ADC is a component that converts a continuous analog voltage into a digital number that a microcontroller can understand. In simple terms, it takes a voltage (like 1.2 V or 2.7 V) and translates it into a numerical value so the ESP32-S3 can process it in code.

The ESP32-S3 has two separate Analog-to-Digital Converter units: **ADC1** and **ADC2**. These ADCs can read a continuous range of voltage values between 0 V and 3.3 V, which makes them ideal for working with sensors  that produce varying signals such as temperature sensors, light sensors, and potentiometers.

Each analog pin is connected to the built-in ADC, which converts the incoming voltage into a digital value. By default, the ESP32-S3 ADC operates at a **12-bit resolution**, meaning it maps the input voltage into a range from **0 to 4095**:
- 0 V → 0 
- 3.3 V→ 4095

Not all GPIO pins are connected to the ADC. Only certain pins can be used to read analog signals on the ESP32-S3, for a total of 20 analog-capable pins.
- **ADC1 pins (recommended): GPIO1 to GPIO10**
- **ADC2 pins: GPIO11 to GPIO20** (these may conflict with Wi-Fi and are less reliable in that case)

<img src="./attachments/esp32s3_analogue.png"/>

### Analog Sensors
#### Potentiometers
A potentiometer is a variable resistor with three terminals that allows us to manually change the voltage going into an analog pin.
- Two outer terminals connect to VCC (3.3V) and GND.
- The middle terminal (wiper) connects to an ADC-capable GPIO like GPIO 1.


When we turn the potentiometer, the voltage at the middle pin changes smoothly from 0V to 3.3V. The ESP32-S3 reads this and gives us a value between 0 and 4095.

Let’s build a project to control the brightness of an LED using a potentiometer. For this project, we will need a potentiometer, an LED, and a 220 Ω resistor. First, connect the potentiometer’s two outer terminals to GND and 3.3V. Then, connect the middle terminal to GPIO 1. Next, connect the short leg of the LED (cathode) to GND, and the long leg (anode) to GPIO 9 through the 220 Ω resistor.

<img src="./attachments/circuit6.png" />

We start by including the libraries required for this project. Since our goal is to control the brightness of an LED, we will use PWM (Pulse Width Modulation). On the ESP32, PWM is handled by the LEDC driver, so we include the necessary headers for LED control, FreeRTOS timing, and ADC functionality. Because we also want to read an analog signal (for example, from a potentiometer), we include the ADC oneshot driver as well.
```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"
```
Next, inside `app_main()`, we configure the PWM timer. The timer defines how the PWM signal behaves its frequency and resolution. In this case, we choose a frequency of 5 kHz, which is fast enough to avoid visible flickering, and a 12-bit resolution, which gives us fine control over brightness (values from 0 to 4095).

We then configure a PWM channel and attach it to GPIO 9, where our LED is connected. Initially, the duty cycle is set to 0, meaning the LED is off.
```c
void app_main() {
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE, 
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_12_BIT, 
        .freq_hz = 5000, 
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE, 
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0, 
        .gpio_num = 9, 
        .duty = 0, 
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
```
Now we move on to configuring the ADC (Analog-to-Digital Converter). The ADC allows the ESP32 to read analog voltages and convert them into digital values that our program can process.   
We use oneshot mode, meaning each conversion is triggered manually whenever we need a reading, rather than running continuously in the background.

First we define the variables that hold its configuration:
- `adc_oneshot_unit_handle_t adc1_handle;`   store a handle (a reference) to the ADC unit after it is initialized. We’ll use it later to access the ADC.
- `adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };`  This structure configures which ADC unit we want to use. The ESP32 has multiple ADC units, and here we select ADC Unit 1.
- `adc_oneshot_chan_cfg_t config = { ... };` This structure defines how a specific ADC channel will behave. It includes:
    - **Bit width** (`.bitwidth = ADC_BITWIDTH_DEFAULT`):  
        This sets the resolution of the ADC. By default, it is 12-bit, meaning the output values range from 0 to 4095.
    - **Attenuation** (`.atten = ADC_ATTEN_DB_12`):  
        This determines the measurable voltage range. With 12 dB attenuation, the ADC can read voltages approximately from 0 V to 3.3 V, which matches typical ESP32 input levels.


Next, we create (initialize) the ADC unit using the configuration with `adc_oneshot_new_unit` which sets up the ADC hardware and stores the resulting handle in `adc1_handle`.
```c
adc_oneshot_new_unit(&init_config1, &adc1_handle);
```
Finally, we configure the specific channel we want to read from:
```c
adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config);
```
We select `ADC_CHANNEL_0` (which maps to GPIO 1) and apply the channel configuration we defined earlier.

Here the full program:
```c
   
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, 
        .atten = ADC_ATTEN_DB_12 
    };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);

    
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config);

    int potValue = 0;
```
Finally, we enter the main loop of the program. This loop runs continuously and performs three simple steps:
1. Read the analog value from the potentiometer using the ADC.
2. Use that value directly as the PWM duty cycle.
3. Update the PWM output so the LED brightness changes accordingly.

Because both the ADC and PWM use a 12-bit range (0–4095), we can directly map the ADC value to the LED brightness without any extra scaling.

A small delay is added to make the system stable and avoid excessive CPU usage.
```c
    
    while (1) {
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &potValue);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, potValue);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}
```
#### Light Dependent Resistor
An LDR (Light Dependent Resistor), or photoresistor, is a special type of resistor whose resistance changes based on the amount of light it is exposed to.
- In **darkness**, its resistance is very high.
- In **bright light**, its resistance becomes very low.

To read its changing resistance, we pair it with a fixed resistor (usually 10k Ω) to create a voltage divider. The output voltage is taken from the point between the two resistors, and its value depends on the ratio of the resistances.
```math
V_{1}​=V_{in}​×\frac{R_1}{R_2​+R_2}​​
```
```math
V_{2}​=V_{in}​×\frac{R_2}{R_2​+R_2}​​
```
![[voltage_divider.png]]

Let’s build an automatic night light that turns on an LED when it gets dark. Connect one leg of the LDR to 3.3V. Connect the other leg to GPIO 1 (ADC1_CH0) and to one end of the 10k Ω resistor. Connect the other end of the 10k Ω resistor to GND. Finally, connect an LED to GPIO 9 through a 220 Ω resistor.

<img src="./attachments/circuit7.png" />


Since we just need standard digital output for the LED, we will use the standard GPIO driver (`driver/gpio.h`).
Now let's make our program, we begin by including the required libraries, then configure GPIO 7 as a digital output to control an LED. The ADC setup is the same as before, where we read an analog value from a sensor .
```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

void app_main() {
   
    gpio_reset_pin(7);
    gpio_set_direction(7, GPIO_MODE_OUTPUT);

    
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, .atten = ADC_ATTEN_DB_12
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config);

    int ldrValue = 0;
```
Inside the main loop, we continuously read the light level and use a simple condition to control the LED. If the measured value is above a chosen threshold (1600), it means the environment is bright, so we turn the LED on. Otherwise, we turn it off. This demonstrates how analog input can be used to trigger digital actions based on conditions.

```c
    while (1) {
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &ldrValue);
        
        
        if (ldrValue > 1600) {
            gpio_set_level(7, 1); 
        } else {
            gpio_set_level(7, 0); 
        }   
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

#### Flame Sensor
The flame sensor detects fire by sensing infrared (IR) light emitted by flames,Since Flames emit radiation across visible and infrared wavelengths. The flame sensor is designed to detect infrared light, typically in the **760–1100 nm** range. Unlike an LDR circuit that usually requires an external voltage divider, the flame sensor module already includes the necessary resistors and signal conditioning circuitry. The module usually has four pins: VCC, GND, DO (Digital Output), and AO (Analog Output).
- **DO pin:** Provides a HIGH or LOW signal to indicate whether a flame is detected, based on a built-in comparator threshold adjusted by a potentiometer on the module.    
- **AO pin:** Outputs a continuous voltage depending on the intensity of the flame.

The sensing element in the module is an infrared photodiode. When infrared radiation from a flame reaches the photodiode, it generates a small electrical signal. The strength of this signal depends on the intensity of the flame. This signal is sent to two different parts of the module:

The **DO pin** provides a simple HIGH or LOW signal to indicate whether a flame is detected. Inside the module, there is a comparator circuit that compares the sensor signal with a reference voltage. This reference voltage can be adjusted using the built-in potentiometer. Turning the potentiometer clockwise makes the sensor more sensitive, allowing it to detect smaller or weaker flames. Turning it counterclockwise makes the sensor less sensitive, so it will only detect stronger or larger flames.
- The DO pin becomes LOW when a flame is detected.
- The DO pin becomes HIGH when there is no flame.

The AO pin outputs a voltage that changes continuously depending on the intensity of the detected infrared radiation.
- If the flame is weak or far away, the output voltage is lower.
- If the flame is strong or closer, the output voltage is higher.


<img src="./attachments/flame_sensor.png"/>

Let’s build a project where a buzzer sounds when a flame is detected. Connect GND and VCC (3.3V) of the sensor to the ESP32-S3. Connect the AO pin to GPIO 1 (ADC1_CH0). Connect the positive pin of an active buzzer to GPIO 7, and its GND to the system ground.

<img src="attachments/circuit8.png" />

Now let’s create our program. We start by including the necessary libraries, then configure GPIO 7 as an output. The ADC is initialized the same way as before to read analog values from the flame sensor.

In the main loop, we continuously read the sensor value. Unlike the LDR, flame sensors often behave inversely: a lower ADC value indicates stronger infrared radiation, which suggests the presence of a flame. So instead of checking for a high value, we check if the reading drops below a certain threshold (1200). If it does, we activate the buzzer; otherwise, it stays off. This simple condition turns the system into a basic fire alert mechanism.
```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

void app_main() {
    gpio_reset_pin(7);
    gpio_set_direction(7, GPIO_MODE_OUTPUT);

    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, .atten = ADC_ATTEN_DB_12
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config);

    int flameValue = 0;

    while (1) {
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &flameValue);
        
        // Lower ADC value means stronger IR detection
        if (flameValue < 1200) {
            gpio_set_level(7, 1); // Sound the buzzer
        } else {
            gpio_set_level(7, 0); // Silence the buzzer
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

#### Soil Moisture Sensor
Soil moisture sensors measure the water level in soil. They fall into two main groups:
- **Resistance-based:** Uses two exposed electrodes. As moisture increases, conductivity increases, lowering resistance.
- **Capacitance-based:** Measures the dielectric permittivity of the soil. Less prone to corrosion.

<img src="./attachments/soil_sensor.png"/>

The resistance-based sensor comes with a module that acts as an interface between the probe and the ESP32-S3. It typically has four pins: VCC, GND, AO (Analog Output), and DO (Digital Output). The AO pin provides a continuous analog voltage corresponding to the measured resistance.
- When the soil is **very wet** we get low resistance, the sensor outputs a **lower voltage**.
- When the soil becomes **drier** get High resistance, the sensor outputs a **higher voltage**.

The DO pin outputs a digital signal (HIGH or LOW) based on a preset threshold, which can be adjusted using the onboard potentiometer; it goes HIGH when the measured value exceeds the set threshold and LOW otherwise.   

Let’s build an automated plant-watering system. We will need the sensor module, a 12V water pump, a 12V power supply, and a 3.3V relay module.

Connect the AO pin of the moisture sensor to GPIO 1. Connect the IN pin of the relay to GPIO 8. Wire the high-voltage side with the 12V pump routing power through the relay's Normally Open (NO) and Common (COM) terminals.

<img src="./attachments/circuit9.png"/>

Now let’s create our program. We start by importing the required libraries, then configure the ADC to read the soil moisture level and set GPIO 8 as a digital output to control the pump.

Inside the loop, the system keeps checking the soil condition. When the soil becomes dry, the pump automatically turns on to water the plant; once enough moisture is detected, it turns off. This way, the plant is watered only when needed, creating a simple automated plant-watering system that reacts directly to the environment.a
```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

void app_main() {
    gpio_reset_pin(8);
    gpio_set_direction(8, GPIO_MODE_OUTPUT);

    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, .atten = ADC_ATTEN_DB_12
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config);

    int moistureValue = 0;

    while (1) {
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &moistureValue);
        
        
        if (moistureValue > 2400) {
            gpio_set_level(8, 1); 
        } else {
            gpio_set_level(8, 0); 
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```