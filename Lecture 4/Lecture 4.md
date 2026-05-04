## Objectives
- Working with Display Devices
- Connectivity and Communication in Esp32s3
## Display Devices
When working on embedded system projects, we often need a way to see what is happening inside our microcontroller.    
This is where display devices come in. Display devices are essential output components that allow the microcontroller to communicate directly with the external world. They help us retrieve information, display sensor data, show alerts, and create user interfaces. Instead of just turning on a single LED to indicate a state, displays allow us to show numbers, text, and even complex graphics.
### LED Matrix
The simplest display device is LED matrix, it is a 2D array of LEDs arranged in rows and columns. The most common size is the 8x8 LED matrix, which contains 64 individual LEDs packed into a single module.  
If we tried to control 64 LEDs individually, we would need 64 digital pins, which most microcontroller does not have. To solve this, the LEDs are wired using a technique called multiplexing. The anodes (positive sides) of the LEDs in each row are connected together, and the cathodes (negative sides) in each column are connected together.

<img src="./attachments/led_matrix.png" height="350px"/>

From the diagram, we can see that a simple 8 × 8 LED matrix would normally require 16 pins to control it (8 rows and 8 columns). For example, sending a high signal to column 1 will activate that column.

<img src="./attachments/led_matrix_1.png" height="300px"/>

Then, by controlling the rows, we can decide which LEDs in that column turn on or off. Suppose we want to turn on only the first two LEDs in column 1. In that case, we would send a high voltage to column 1 and, to select the rows signals we send High signal to all rows except the first and second one so only those two LEDs are active, while the others remain off.

<img src="./attachments/led_matrix_2.png"  height="300px"/>

However, when we try to draw more complex shapes, we may encounter a problem. We might run into conflicts where turning on specific LEDs causes other unwanted LEDs to light up. As a result, the displayed image may appear incomplete or incorrect.
To solve this issue, we can split the drawing into multiple frames. For example, to draw a smiling face, the first frame could display the eyes, and the second frame could display the mouth. By switching very quickly between these frames, we create the illusion that both parts are displayed simultaneously. Because of the persistence of vision, the observer perceives the full smiling face instead of separate images.

<img src="./attachments/led_matrix_3.png" height="250px" />

Following this method we can draw most of the shape we want but there is small problem using all 16 pins would consume most of the available pins on our microcontroller, to simplify working with an LED matrix, we use a driver chips such as the MAX7219. This chip handles all the multiplexing internally, so we don't need to control each LED individually. As a result, we will need only three digital pins Data, Clock, and Load (CS).
#### Using MAX7219 
The MAX7219 driver helps us control an LED matrix while reducing the number of control pins to only three. To manage all the LEDs, it contains an internal 8×8 memory (similar to SRAM) that stores which LEDs should be on. Its internal circuitry then translates this data and applies it to the LED matrix. We communicate with this driver using SPI communication, where the data flow is controlled using three pins:

- **Data pin (DIN)**: This is where the actual information flows in. We send one bit at a time. The first 8 bits represent the register address, and the next 8 bits represent the value or pattern.
- **Clock pin (CLK)**: This provides the timing. On each rising edge of the clock signal, the MAX7219 samples and shifts in one bit from the DIN line into its internal shift register.
- **CS / LOAD pin**: This acts as the latch or chip select signal (active low). While CS is held **low**, the chip listens and shifts in bits with every CLK pulse. After exactly 16 clock pulses (16 bits sent), we pull CS high. This rising edge tells the MAX7219, “OK, the full command is ready now decode it and apply it to the display or registers.”

In short, to communicate with the MAX7219, we first pull CS (Chip Select) low to indicate that data transmission is starting. Then, for 16 clock cycles, we set the desired bit on the DIN (Data) pin and pulse the CLK (Clock) pin high and then low. Each clock pulse sends one bit to the chip. After the 16th clock pulse, we pull CS high. The MAX7219 then latches the 16 bits, interprets them as an address and data, and updates the display memory or configuration settings such as brightness or scan limit. Once this is done, the MAX7219 automatically handles the continuous row-by-row scanning of the LED matrix, Here diagram illustrate the operation.

<img src="./attachments/diagram.png" height="300px"/>

Let’s create a simple project to display a heart on the LED matrix. First, we need to build the circuit.We start by connecting the 5v and GND pins of the ESP32s3 to the 5v and GND pins of the MAX7219 module.  Next, We connect the command pins as following
- CLK to Esp32s3 GPIO 19
- CS to Esp32s3 GPIO 20
- DIN to Esp32s3 GPIO 21

<img src="./attachments/circuit_4_1.png"/>


Now let’s create our program we start by including the library that we will need
```c
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
```
After that we create constant that represent our pins
```c
#define DIN 21
#define CLK 19
#define CS  20
```
After that we create function that handel the data-sending logic. Since the hardware interface sends data one bit at a time, the byte must be broken down into its individual bits before transmission. We manage that by using a masking technique with bitwise operations. Inside the function, we define an array of masks:  
```c
{0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}.  
```
Each mask corresponds to one bit position in the byte, starting from the most significant bit (MSB) to the least significant bit (LSB). For example, `0x80` (binary `10000000`) isolates the first bit, while `0x01` (binary `00000001`) isolates the last bit.

During each loop iteration, we use the bitwise AND operation `(data & bits[i])` to extract a specific bit from the byte. The idea is simple: the AND operation keeps only the bit that matches the mask and clears all others. If the result is not zero, it means that particular bit in `data` is `1`; otherwise, it is `0`.

After extracting the bit value, we send it through the data pin (`DIN`). If the bit is `1`, we set the pin high; if it is `0`, we set it low. At the same time, we control the clock signal (`CLK`) to synchronize the transmission. The clock is first set low, then the data bit is placed on the data line, and finally the clock is set high.    
By repeating this process for all 8 bits, the function successfully transmits the entire byte, one bit at a time, using precise control of both data and clock lines.
```c
void send_byte(uint8_t data){
   const uint8_t bits[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    for (int i = 0; i < 8; i++) {
        gpio_set_level(CLK, 0);
        if ((data & bits[i]) != 0)
            gpio_set_level(DIN, 1);
        else
            gpio_set_level(DIN, 0);

        ets_delay_us(2);
        gpio_set_level(CLK, 1);
        ets_delay_us(2);
    }
}
```
After finishing the function responsible for sending a single byte, the next step is to build a higher-level function that handles communication with the MAX7219 driver. 

The MAX7219 expects data in a 16-bit format, which is divided into two bytes. The first byte represents the register address, indicating which internal register we want to access (for example, a digit or a control register). The second byte contains the actual data that will be written to that register.

To handle this, the function takes two arguments: `reg` for the register address and `data` for the value to send. The communication begins by pulling the Chip Select (`CS`) line low. This signals the MAX7219 that a transmission is starting and that it should pay attention to the incoming data.

Once communication is initiated, we send the register address first using the previously defined `send_byte` function. This ensures the driver knows where the incoming data should be stored. Immediately after that, we send the data byte itself, again using the same byte-sending function.

Finally, after both bytes have been transmitted, we set the `CS` line high. This rising edge tells the MAX7219 to latch the received 16-bit data into its internal registers, effectively completing the operation.
```c
void max7219_send(uint8_t reg, uint8_t data){

    gpio_set_level(CS, 0);

    send_byte(reg);
    send_byte(data);

    gpio_set_level(CS, 1);

}
```

With the communication function in place, the next step is to initialize the MAX7219 so it operates in the desired mode. This is done by sending a sequence of configuration commands to its internal registers using the `max7219_send` function we created earlier.

Each call to `max7219_send(reg, data)` writes a value to a specific register inside the driver. The initialization function starts by configuring the display test register (`0x0F`) with `0x00`, which disables test mode. This is important because test mode forces all LEDs on.

Next, the shutdown register (`0x0C`) is set to `0x01`. Despite its name, this actually brings the device out of shutdown mode and turns the display on. Without this step, the MAX7219 would remain inactive.

Then, the scan limit register (`0x0B`) is set to `0x07`. This tells the driver to use all 8 digits (from 0 to 7). If a smaller value were used, only part of the display would be active.

After that, the intensity register (`0x0A`) is set to `0x08`, which controls the brightness of the LEDs. The value can typically range from `0x00` (dimmest) to `0x0F` (brightest), so this sets a medium brightness level.

The decode mode register (`0x09`) is then set to `0x00`, which disables BCD decoding. This means we are working in raw mode, where each bit directly controls an LED segment.

Finally, a loop runs through all the 8 rows (from 1 to 8) and clears them by sending `0x00`. This ensures that the display starts in a clean state with all LEDs turned off.
```c
void max7219_init(){
    max7219_send(0x0F, 0x00);
    max7219_send(0x0C, 0x01);
    max7219_send(0x0B, 0x07);
    max7219_send(0x0A, 0x08);
    max7219_send(0x09, 0x00);
  

    for (int i = 1; i <= 8; i++)
        max7219_send(i, 0x00);

}
```
With all the building blocks ready, we can now implement the main logic of the program inside `app_main`. 

At the beginning of the function, we configure the GPIO pins connected to the MAX7219 `DIN`, `CLK`, and `CS` as outputs. We then set the `CS` (Chip Select) line high, which means the device is idle and not currently receiving data.

Next, we call the `max7219_init()` function to initialize the driver with the configuration we defined earlier. 

After initialization, we define an array called `heart` that holds 8 bytes. Each byte represents one row of the LED matrix, and each bit inside the byte corresponds to a single LED in that row. A value of `1` means the LED is on, while `0` means it is off. The pattern of binary values in this array forms the shape of a heart when displayed on the matrix.

Inside the infinite loop, we first iterate through the `heart` array. For each row, we call `max7219_send(i + 1, heart[i])`. The `i + 1` represents the row, and `heart[i]` is the pattern for that row. 

After displaying the heart, we introduce a delay of 500 milliseconds using `vTaskDelay`. This keeps the heart visible for a short time.

Then, we run another loop that clears the display by sending `0` to all rows. This effectively turns off all LEDs. Another 500 ms delay follows, keeping the display blank for the same duration.

By continuously repeating these two steps displaying the heart and then clearing it we create a blinking heart effect on the LED matrix.
```c
void app_main(void) {

    gpio_set_direction(DIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(CS, GPIO_MODE_OUTPUT);

    gpio_set_level(CS, 1);

    max7219_init();

    uint8_t  heart[8] = {
    0B00000000,
    0B01100110,
    0B11111111,
    0B11111111,
    0B11111111,
    0B01111110,
    0B00111100,
    0B00011000
  };
    while (1) {
        for (int i = 0; i < 8; i++) {
            max7219_send(i + 1, heart[i]);
        }
        
        vTaskDelay(pdMS_TO_TICKS(500));

        for (int i = 0; i < 8; i++) {
           max7219_send(i + 1, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```
### Seven Segment Display
Seven-segment displays are digital displays made up of seven LED segments arranged to form numbers 0-9 and some letters. These segments are labeled with letters from A to G. By turning on specific combinations of these segments, we can form any number.     
For example, to display the number "1", we turn on segments B and C.  

<img src="./attachments/7segments.png" height="300px">

There are two main types of seven-segment displays:
- **Common Cathode:** All the negative pins (GND) of the LEDs are connected together. We send a HIGH signal to a segment's pin to turn it ON.
- **Common Anode:** All the positive pins (VCC) of the LEDs are connected together. We send a LOW signal to a segment's pin to turn it ON.

If we try to control each segment individually, we would need 7 GPIO pins. This works fine for a single display, but when using multiple 7-segment digits, we would quickly run out of available GPIO pins.

To address this, many 7-segment displays are paired with a BCD-to-7-segment decoder. This approach reduces the number of required GPIO pins from 7 down to just 4 control pins. Instead of driving each segment directly, we send the digit value using BCD (Binary-Coded Decimal) encoding, where each decimal digit is represented by its binary equivalent.

The decoder then translates this binary input into the appropriate signals to illuminate the correct segments. The corresponding mapping can be seen in the table below:

|Decimal Digit|BCD (a,b,c,d)|
|---|---|
|0|0000|
|1|0001|
|2|0010|
|3|0011|
|4|0100|
|5|0101|
|6|0110|
|7|0111|
|8|1000|
|9|1001|

For example, with a common cathode 7-segment display, we can display the digit "3" by sending a BCD signal  of (low, low, high, high), as shown below:  

<img src="./attachments/7segment_bcd.png" height ="350px"/>

### Four-digit 7-segment
In many applications, we need to display numbers with more than one digit, such as two-digit, four-digit, or larger values. A four-digit 7-segment display is formed by combining four individual 7-segment digits into a single module. This is achieved by connecting all corresponding segment pins together, while adding a separate control (select) pin for each digit to determine which one is active at any given time.

<img src="./attachments/4digit_7segments.png" height="400px"/>

If we tried to control all four digits manually, we would need 12 pins: 8 pins for segments and 4 pins for digit selection. To reduce the number of pins, we can use a BCD encoder, which allows us to drive all four digits using only 9 pins (4 for BCD input + 5 for segments including the decimal point) we can use 8 if we remove the decimal point.

<img src="./attachments/4digit_7segments_bcd.png" height="450px"/>

#### Working with TM1637 
The TM1637 is a dedicated LED drive control circuit commonly used with four-digit 7-segment displays. Much like the MAX7219, its primary goal is to drastically reduce the number of GPIO pins needed to drive the display. While a standard 4-digit display might require up to 12 pins, the TM1637 reduces this to just two communication pins, alongside power and ground.

We communicate with the TM1637 using a 2-wire serial protocol that is very similar to I2C, though it lacks device addressing. The data flow is managed using these two pins:
- **Data pin (DIO):** This bidirectional pin is used to transfer data into and out of the chip. We send commands and display data over this line, one bit at a time.
- **Clock pin (CLK):** This provides the timing for the data transfer. Data on the DIO pin must be stable while the clock is high, and it can only change state while the clock is low.

Unlike MAX7219 which relies on a Chip Select (CS) pin to indicate the start and end of a transmission, the TM1637 relies on specific signaling conditions on the data and clock lines:
- START condition: The transmission begins when the DIO line is pulled LOW while the CLK line remains HIGH.
- STOP condition: The transmission ends when the DIO line is pulled HIGH while the CLK line is HIGH.
- **Data Transmission:** Between the start and stop conditions, 8 bits of data are sent, Least Significant Bit (LSB) first. After the 8 bits are sent, a 9th clock pulse is generated to allow the TM1637 to send an Acknowledge (ACK) signal by pulling the DIO line low, after that we can set the stop condition to stop data transmission or we can send another pack of data.


<img src="./attachments/diagram2.png" />

Let’s create a simple project to display the number "1234" on our 4-digit 7-segment display. We start by connecting the 5v and GND pins of the ESP32-S3 to the VCC and GND pins of the TM1637 module. Next, we connect the communication pins as follows:
- CLK to ESP32-S3 GPIO 19
- DIO to ESP32-S3 GPIO 21

<img src="./attachments/4_7segments_circuit.png" height="350px" />


Now let’s write our program. We start by including the necessary libraries.
```c
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
```
After that, we create the constants that represent our pins.
```
#define CLK 19
#define DIO 21
```
Because the TM1637 protocol relies on Start and Stop conditions rather than a CS pin, we need to create two small helper functions to handle these specific sequences. For the Start condition, DIO goes low while CLK is high. For the Stop condition, DIO goes high while CLK is high.
```c
void tm1637_start(void) {
    gpio_set_level(CLK, 1);
    gpio_set_level(DIO, 1);
    ets_delay_us(2);
    gpio_set_level(DIO, 0);
    ets_delay_us(2);
    gpio_set_level(CLK, 0);
}

void tm1637_stop(void) {
    gpio_set_level(CLK, 0);
    gpio_set_level(DIO, 0);
    ets_delay_us(2);
    gpio_set_level(CLK, 1);
    ets_delay_us(2);
    gpio_set_level(DIO, 1);
}
```
Next, we implement the function responsible for sending a single byte. Unlike the MAX7219, which expects data to be transmitted Most Significant Bit (MSB) first, the TM1637 requires the Least Significant Bit (LSB) to be sent first. To accommodate this, we reverse the bit extraction logic used previously. Instead of iterating array of mask from the first to last, we loop through them in the opposite direction.    
After all 8 bits are transmitted, the TM1637 requires an acknowledgment (ACK) phase. During this phase, the device pulls the `DIO` line low to indicate it successfully received the byte. To handle this, we generate an extra (9th) clock pulse, and set the data line low to complete the cycle, without explicitly checking the ACK signal.
```c
void tm1637_write_byte(uint8_t data) {
    const uint8_t bits[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    for (int i = 7; i >= 0; i--) {
        gpio_set_level(CLK, 0);
        if (data & bits[i]) {
            gpio_set_level(DIO, 1);
        } else {
            gpio_set_level(DIO, 0);
        }
        ets_delay_us(2);
        gpio_set_level(CLK, 1);
        ets_delay_us(2);
    }
    // aknowledge cycle 1 pulse
    gpio_set_level(CLK, 0);
    gpio_set_level(DIO, 0);
    ets_delay_us(2);
    gpio_set_level(CLK, 1);
    ets_delay_us(2);
    gpio_set_level(CLK, 0);
}
```
To display numbers on a 7-segment display, we need to know which segments to turn on for each digit (0-9). For that we can define an array where the index corresponds to the digit, and the value is the hexadecimal representation of the segments that need to be turned on.
```c
const uint8_t segment_map[] = {
    0x3F, // 0: Segments A,B,C,D,E,F
    0x06, // 1: Segments B,C
    0x5B, // 2: Segments A,B,D,E,G
    0x4F, // 3: Segments A,B,C,D,G
    0x66, // 4: Segments B,C,F,G
    0x6D, // 5: Segments A,C,D,F,G
    0x7D, // 6: Segments A,C,D,E,F,G
    0x07, // 7: Segments A,B,C
    0x7F, // 8: Segments A,B,C,D,E,F,G
    0x6F  // 9: Segments A,B,C,D,F,G
};
```
Now we can put everything together and implement the main application logic for the TM1637 display. 

At the start of `app_main`, we configure the `CLK` and `DIO` pins as outputs. These two lines form the communication interface with the TM1637. After that, we set both lines high, which represents the idle state of the bus. This is important because the TM1637 protocol expects both clock and data lines to be high when no transmission is taking place.

The communication is divided into three main commands, each wrapped between a start and stop condition using `tm1637_start()` and `tm1637_stop()`. These functions define the beginning and end of each transmission.

The first command sends `0x40`, which configures the TM1637 for automatic address increment mode. This means that after writing one byte of data, the internal address pointer automatically moves to the next position, making it easier to send multiple digits in sequence.

The second command controls the display state and brightness. The value `0x8F` combines two settings: turning the display on (`0x88`) and setting the brightness to its maximum level (`0x07`). This ensures the digits are visible and clearly lit.

Inside the infinit loop the third command begins by sending `0xC0`, which sets the starting address for display data (the first digit position). After that, we send four bytes, each representing a digit to be displayed. These values come from the `segment_map` array.

After sending all commands, we add a delay of 1 second using `vTaskDelay`. This keeps the displayed numbers stable before the loop repeats and refreshes the display again.
```c
void app_main(void) {
    
    // Configure pins as outputs
    gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIO, GPIO_MODE_OUTPUT);
    
    // Set initial state high (idle state)
    gpio_set_level(CLK, 1);
    gpio_set_level(DIO, 1);

    // Command 1: Write data, auto-increment address
    tm1637_start();
    tm1637_write_byte(0x40); 
    tm1637_stop();
    
    // Command 2: Display ON, set brightness
    tm1637_start();
    tm1637_write_byte(0x8F); 
    tm1637_stop();

    while (1) {
        
        // Command 3: Set start address (0xC0) and send the 4 digits
        tm1637_start();
        tm1637_write_byte(0xC0); 
        tm1637_write_byte(segment_map[1]); // Send pattern for "1"
        tm1637_write_byte(segment_map[2]); // Send pattern for "2"
        tm1637_write_byte(segment_map[3]); // Send pattern for "3"
        tm1637_write_byte(segment_map[4]); // Send pattern for "4"
        tm1637_stop();
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```
To make the project more interesting, we modify the main logic so the display shows a countdown from 59 to 0 instead of fixed digits. The overall structure of the program remains the same we keep the initialization and communication setup exactly as before, and only update the display logic.

Inside the infinite loop, we introduce a `for` loop that counts down from 59 to 0. This loop controls the value we want to display on the 4-digit screen. For each iteration, we send a new value to the display.

The display update starts with the usual start condition and the `0xC0` command to set the starting address. Then we send four bytes, each representing one digit:
- The first two digits are set to `0`, so they display `00`. This keeps the focus on the last two digits.
- The third digit is set using `i / 10`. This extracts the tens place of the number. For example, if `i = 47`, then `47 / 10 = 4`.
- The fourth digit is set using `i % 10`. This extracts the units place. For example, `47 % 10 = 7`.

By combining these two operations, we split the number into its decimal digits and display them correctly on the 7-segment display.

After sending the digits, we stop the transmission and wait for 1 second using `vTaskDelay`. This creates the visible countdown effect, decreasing one number every second.

```c
void app_main(void) {
    
    gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIO, GPIO_MODE_OUTPUT);
    
    gpio_set_level(CLK, 1);
    gpio_set_level(DIO, 1);

    tm1637_start();
    tm1637_write_byte(0x40); 
    tm1637_stop();
        
    tm1637_start();
    tm1637_write_byte(0x8F); 
    tm1637_stop();
    
    while (1) {
        for (int i = 59; i >= 0; i--){
          tm1637_start();
          tm1637_write_byte(0xC0); 
          tm1637_write_byte(segment_map[0]);
          tm1637_write_byte(segment_map[0]);
          tm1637_write_byte(segment_map[i/10]);
          tm1637_write_byte(segment_map[i%10]);
          tm1637_stop();
          vTaskDelay(pdMS_TO_TICKS(1000));
        }   
    }
}
```
### LCD Display
LED matrices and seven-segment displays are very useful when we need to display numbers or simple shapes. However, they have an important limitation. Seven-segment displays are mainly designed for numbers, and LED matrices require a large number of LEDs and complex control if we want to display readable text. To solve this problem, we use LCD displays (Liquid Crystal Displays). These displays allow us to show text characters and sometimes small symbols, making them ideal for building user interfaces.

An LCD (Liquid Crystal Display) works using a special material called liquid crystals. These materials have properties between a liquid and a solid crystal.

Inside the LCD module, there are several layers:
1. Two polarizing filters
2. A layer of liquid crystals
3. Glass plates with transparent electrodes
4. A backlighz

<img src="./attachments/lcd.png" height="350px"/>
Unlike LED displays, LCD pixels do not emit light themselves. Instead, they act as tiny shutters that control how light passes through the screen.When the internal controller applies a voltage to specific pixel areas, the liquid crystals physically twist. This twisting changes the polarization of the light. While light passes through the crystals naturally when no electricity is applied, the twisted crystals cause the second (front) polarizing filter to block the light. As a result, the pixel appears dark, while the surrounding areas remain bright. This blocked light creates the dark "pixels" that form our letters, numbers, and symbols.      
There is multiple modules of LCD but the most common modules available in the market are:
- **16 × 2 LCD** Displays 16 characters per row and 2 rows.
- **20 × 4 LCD** Displays 20 characters per row and 4 rows.
- **40 × 2 LCD** Displays 40 characters per row and 2 rows.
- **40 × 4 LCD** Displays 40 characters per row and 4 rows.

A standard 16 × 2 LCD module typically has 16 pins. These pins are used for power, control signals, data communication, and backlight control. 

<img src="./attachments/lcd_16_2.png" height="400px"/>

To communicate with the LCD directly, we have two possible modes:
- 8-bit mode
- 4-bit mode

In 8-bit mode, we uses all eight data pins (D0–D7) of the LCD to send one full byte of data at a time. This method is faster because the entire character is transmitted in a single operation. However, this mode requires many GPIO pins:
- 8 pins for data
- 2 control pins (RS, Enable)

In 4-bit mode, the LCD receives data in two steps instead of one. First, we sends the higher 4 bits, then we sends the lower 4 bits of the same byte. The LCD internally combines these two parts to reconstruct the full 8-bit value.  
This approach reduces the number of required pins:
- 4 data pins (D4–D7)
- 2 control pins (RS and Enable)

Let’s create a simple “Hello World” example using a 16×2 LCD in 4-bit mode. First, connect the LCD to the ESP32-S3: connect RS to GPIO 20, EN to GPIO 19, D4 to GPIO 1, D5 to GPIO 2, D6 to GPIO 42, and D7 to GPIO 41. For power connections, connect VSS, RW, and K to GND, and VDD and A to 5V. 

<img src="./attachments/lcd_16_2_circuit.png"/>

Now Lets create LCD component, we start by adding a `components` directory to our project root. Inside it, create a folder named `lcd` to house all files related to the LCD driver.   
Within the `lcd` folder, we add a `CMakeLists.txt` file to define the build rules, an `lcd.c` file for the implementation, and an `include` directory for the `lcd.h` header file.

```
├── CMakeLists.txt 
├── main/ 
│  ├── CMakeLists.txt 
│  └── main.c 
└── components/ 
   └── lcd/ 
      ├── CMakeLists.txt 
      ├── lcd.c 
      └── include/ 
         └── lcd.h
```
Inside `components/lcd/CMakeLists.txt`, we use the `idf_component_register` function to tell the build system which source files to compile and where to find the public headers:
```
idf_component_register(SRCS "lcd.c"  
INCLUDE_DIRS "include")
```
Finally, we update the `CMakeLists.txt` file inside the `main` folder. By adding the `REQUIRES` keyword, we ensure the build system links the `lcd` component to our main application:
```
idf_component_register(SRCS "main.c"  
INCLUDE_DIRS "."
REQUIRES lcd)
```
After finishing restructuring the project we can start implementing the lcd component logic, First we read the datasheet to know how we can program and control the divice.   
By quick check we can see that the RS and RW pins controll the working mode of the lcd:
- RS=0 and RW=0 the lcd is lestening to configuration commands.
- RS=1 and RW=0 the lcd is reciving comands to display

Also from the datasheet we can see that the command are sent using the data pins D0...D7, and the lcd accept the following commads

<img src="./attachments/config_table.png" />


For each command we sent , we need to send a pulse on the E (enable) pins to validate the data, the following diagram illustrate how the signal should be sent.

<img src="./attachments/time_diagram.png" />


Finally before sending any command or trying to display any character in the lcd we need to first wait 15 ms and initlialize it by setting the D5 and D4 to 1 and sending three Enable pulse , after that the first command we need to send is the display mode, we set it to 4 bits after that we can start configuring our lcd by sending the rest of the commands, The following chart display the workflow

<img src="./attachments/chart.png" />

Now we know how the Lcd work, lets start creating our program, we start by including the library we will use 
```c
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
```
Now lets create our functions we start with `lcd_pulse_enable` which handel the enable pulse on the E pin
```c
void lcd_pulse_enable() {
    gpio_set_level(E, 1);
    ets_delay_us(1);
    gpio_set_level(E, 0);
    ets_delay_us(20);
}
```
Since we planning to use the 4 bits mode lets create function responsible of sending 4 bit to the LCD, the function take as argument  data of type byte, and since we can send only 1 bit at once to our pins we use bitwise and with mask to extract our pits
```c
void lcd_send_bits(uint8_t bits){
	gpio_set_level(D4, bits & 0b1);
	gpio_set_level(D5, bits & 0b10);
	gpio_set_level(D6, bits & 0b100);
	gpio_set_level(D7, bits & 0b1000);
	lcd_pulse_enable();
}
```
We now implement the function responsible for sending a command, in 4-bit mode, the LCD expects data in two steps: first the upper 4 bits (high nibble), then the lower 4 bits (low nibble).
To extract the first the upper 4 bits we shift right by 4.
```c
void send_command(uint8_t command){
	lcd_send_bits(command >> 4);
	lcd_send_bits(command );
}
```
We create function to initialize our GPIO pins
```c
void lcd_gpio_init(){
  // 1. Setup GPIOs
    gpio_set_direction(D4, GPIO_MODE_OUTPUT);
    gpio_set_direction(D5, GPIO_MODE_OUTPUT);
    gpio_set_direction(D6, GPIO_MODE_OUTPUT);
    gpio_set_direction(D7, GPIO_MODE_OUTPUT);
    gpio_set_direction(RS, GPIO_MODE_OUTPUT);
    gpio_set_direction(E, GPIO_MODE_OUTPUT);

    // Initial State
    gpio_set_level(RS, 0);
    gpio_set_level(E, 0);
}
```
Now that we have built the backbone for LCD communication, we can implement the initialization function.  This function prepares the LCD to operate correctly in 4-bit mode.

When the LCD powers up, it defaults to 8-bit mode. To switch it into 4-bit mode, we must follow a specific initialization sequence defined in the controller datasheet . First, we wait about 50 ms to ensure the LCD has fully powered up. Then, we send the value `0x03` three times using (D4 and D5), each followed by an enable pulse. This step forces the LCD into a known 8-bit state, regardless of its previous condition.   
After that, we send `0x02` to switch the LCD into 4-bit mode.   
Once the LCD is in 4-bit mode, we can send full commands to configure the display. We begin with the Function Set command to select a 4-bit interface and 2-line display mode. Next, we turn the display on while disabling the cursor and blinking. Finally, we clear the display and wait for the operation to complete.
```c
void initialize_lcd(){ 
   lcd_gpio_init();
   // wait lcd powered 
   vTaskDelay(pdMS_TO_TICKS(50)); 

   // setting D4 D5 as hight for three enable pulse 
   lcd_send_bits(0x03); 
   vTaskDelay(pdMS_TO_TICKS(5)); 

   lcd_pulse_enable(); 
   ets_delay_us(100);

   lcd_pulse_enable(); 
   ets_delay_us(10); 


   // setting the command mode to be 4 bits 
   lcd_send_bits(0x02);
   
   // Function Set we set the data lenght to 4 bits and display to be 2 lines 
   //Dl=0 
   //N = 1 
   send_command(0x28); 
   //Now we set display to be on and cursor and blinking cursor to be off 
   send_command(0x0C); 
   //finnally we clear the display and return to position 0 0 
   send_command(0x01); 
   vTaskDelay(pdMS_TO_TICKS(2)); 
   
  }
```
Before implementing the functions used to display text and characters, we first define two helper functions.   
The first function clears the display and returns the cursor to the home position. This is done by sending the clear display command (`0x01`), followed by a short delay to allow the LCD to complete the operation.
```C
void clear display(){
	send_command(0x01);
    vTaskDelay(pdMS_TO_TICKS(2));
}
```
The second helper function moves the cursor to a specific position, defined by the line and column. This works by setting the highest bit (D7) to 1 and combining it with the target DDRAM address, as illustrated in the table below.

For most 16×2 LCDs:
- Line 1 starts at address `0x00` → command base `0x80`
- Line 2 starts at address `0x40` → command base `0xC0`

<img src="./attachments/address.png" />

```c
void move_to(uint8_t line, uint8_t column){
	if (line == 1){
	send_command(0x80 | column);
	}else{
		send_command(0xC0 | column);
	}
}
```
Now we can implement the function responsible for displaying a single character. This function takes a character as an argument, sets the RS (Register Select) pin high to indicate that we are sending data (not a command), sends the character to the LCD, and then sets RS back to low.
```c
void write_character(char c){
  gpio_set_level(RS, 1);
  send_command(c);
  gpio_set_level(RS, 0);
}
```
We can also create a function to display a full message (string). This function takes an array of characters and uses a loop to iterate through each character until it reaches the null terminator (`'\0'`). Each character is then sent to the LCD using the `write_character` function.
```c
void write_word(char message[]){
  for (int i=0;message[i]!='\0';i++){
    write_character(message[i]);
  }
}
```
The final step is creating the `lcd.h` header file. This file defines the public interface of our LCD driver which allow other part of the program to interact with the LCD.

We first use include guards to prevent the header from being included multiple times, which can cause errors. This is done with `#ifndef`, `#define` at the top, and `#endif` at the bottom.   
between  the guards we write the functions we plan to share. and we define the constants that represent our pins
```c
#ifndef LCD_H  
#define LCD_H  
  
#include <stdint.h>  
  
#define D4 1
#define D5 2
#define D6 42
#define D7 41
#define RS 20
#define E 19

// Initialization  
void initialize_lcd(void);  
  
// Basic control  
void clear_display(void);  
void move_to(uint8_t line, uint8_t column);  
  
void write_character(char c);  
void write_word(char message[]);  
  
#endif
```
Now, in the `main.c` file, we can include the LCD component and display a simple message.

After initializing the LCD, we send a string to be displayed on the screen.
```c
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lcd.h"  
  
void app_main(void) {
    initialize_lcd();
    write_word("Hello world");

}
```