## Objectives
- Explore the ESP ecosystem
- Understand ESP architecture and development boards
- Learn C programming concepts and create a blinking LED project

## Exploring ESP Microcontrollers
### Introduction
Microcontrollers are powerful devices that allow us to build smart and automated systems. While platforms like Arduino made learning electronics accessible, the modern world demands connectivity. Today’s projects often need to connect to the internet, stream data to the cloud, or communicate with our smartphones. Adding these capabilities to traditional basic microcontrollers used to be complex and expensive.

To solve this, Espressif Systems introduced a line of low-cost, highly integrated microcontrollers that revolutionized the Internet of Things (IoT): the ESP series.
### What Is an ESP?
The ESP family consists of low-power, highly integrated microchips featuring built-in Wi-Fi and Bluetooth capabilities. Instead of needing a separate microcontroller and a separate wireless module, ESP chips pack both into a single piece of silicon.

Because of their immense processing power, generous memory, and built-in wireless features, they have become the industry standard for IoT devices, smart home automation, and advanced DIY electronics. Furthermore, they are highly supported by the open-source community, making the transition for beginners incredibly smooth.
### Bare Chip vs. Module vs. Dev Kit
The ESP products are commonly found in three different forms. Understanding this hierarchy is crucial:

#### The Bare Chip: 
This is the raw, tiny silicon chip itself (e.g., the ESP32-D0WDQ6). It is very difficult to work with directly because it requires you to design a complex circuit around it, including external flash memory, a crystal oscillator, and an antenna.

<img src="./attachments/bare_chip.png" />

#### The Module: 
To make the chip usable for manufacturers, Espressif packages the bare chip onto a small stamp-sized circuit board alongside flash memory, a crystal, and an integrated trace antenna (usually covered by a shiny metal shield). A famous example is the **ESP-WROOM-32** module. While easier to use, it still requires soldering to a custom circuit board and operates strictly at 3.3V.

<img src="./attachments/esp-wroom-32.png" />

#### The Development Kit (Dev Kit): 
A Dev Kit takes the ESP module and solders it onto a larger, breadboard-friendly circuit board. It adds a USB port, a USB-to-Serial converter, a 3.3V voltage regulator, and easily accessible pin headers, making it the best option for learning and prototyping because it is easier to power, program, and connect to other components.

<img src="./attachments/DevKit.png" />


### The ESP Series
Espressif has released several generations of chips, each tailored for different technological needs.
#### ESP8266
The ESP8266 is the chip that started the IoT revolution. Before it arrived, adding Wi-Fi to a microcontroller cost upwards of $30. The ESP8266 did it for about $2. It features a single-core 32-bit processor and built-in Wi-Fi. While it is older and has fewer input/output pins than its successors, it remains incredibly popular for simple, low-cost smart home sensors and switches. Common Dev Kits for this chip include the NodeMCU and the Wemos D1 Mini.

<img src="./attachments/esp8266.png"/>

#### ESP32 
The original ESP32 is the powerhouse successor to the 8266. It upgraded almost everything: it features a dual-core 32-bit processor, adds Bluetooth both Classic and Low Energy, drastically increases the number of GPIO pins, and includes hardware encryption. It is the gold standard for projects requiring heavy lifting, such as audio streaming, complex web servers, or handling multiple sensors simultaneously.
#### ESP32-S Series (S2, S3)
The ESP32-S series focuses on security, specialized features, and advanced processing.
- **ESP32-S2:** A single-core Wi-Fi chip that introduced native USB support meaning it can act like a mouse or keyboard,  and enhanced security features.
- **ESP32-S3:** A dual-core beast designed specifically for AI and Machine Learning at the edge. It includes "vector instructions" that dramatically speed up neural networks, making it ideal for voice recognition or facial detection cameras.

<img src="./attachments/esp32s3.png" />

#### ESP32-C Series 
The ESP32-C Series represents a shift in Espressif’s design philosophy. Unlike earlier chips based on the proprietary Xtensa architecture, the C-series uses the open-source RISC‑V architecture. These chips focus on low power consumption, cost efficiency, and modern wireless connectivity, making them ideal for IoT devices.
- **ESP32-C2:**  An entry-level, ultra-low-cost microcontroller designed for simple IoT applications. It provides Wi-Fi 4 (2.4 GHz) connectivity and basic security features while maintaining very low power consumption, making it suitable for mass-produced connected devices.
- **ESP32-C3:** A popular and widely used chip that serves as a modern replacement for the **ESP8266**. It features a single-core RISC-V CPU, Wi-Fi 4, and Bluetooth LE 5.0, offering improved security, performance, and low power usage for general IoT projects.
- **ESP32-C5:**  Introduces dual-band Wi-Fi 6 (2.4 GHz and 5 GHz) support, delivering higher throughput and lower latency compared to earlier chips. It is designed for more demanding wireless applications where faster and more reliable connectivity is required.
- **ESP32-C6:**  Targets next-generation IoT ecosystems by combining Wi-Fi 6, Bluetooth LE 5.3, and support for Zigbee and Thread protocols. This makes it particularly well suited for smart home and Matter-compatible devices that need multiple wireless standards.

#### ESP32-H Series 
The ESP32-H Series is designed specifically for low-power mesh networking used in modern smart home ecosystems. These chips focus on IEEE 802.15.4 protocols rather than Wi-Fi.
- **ESP32-H2:**  A low-power RISC-V–based microcontroller built for devices that rely on Thread, Zigbee, and other IEEE 802.15.4 protocols. It also includes Bluetooth LE 5.2 for provisioning and device communication.  Unlike most ESP32 chips, it does not include Wi-Fi, making it ideal for Matter-compatible smart home devices, sensors, and battery-powered mesh nodes.

- **ESP32-H4:**  Designed as a Bluetooth-focused companion MCU for IoT systems. It targets Bluetooth Low Energy applications such as wearable devices, sensors, and gateways. The H4 can be used alongside other ESP chips (for example Wi-Fi MCUs) to offload Bluetooth connectivity and wireless protocol handling, improving efficiency in multi-radio systems.

#### ESP32-P Series 
The ESP32-P Series targets applications that need significantly more processing power than typical IoT microcontrollers.

- **ESP32-P4:**  A powerful RISC-V–based MCU designed for compute-heavy embedded systems such as advanced HMIs, multimedia processing, and edge AI workloads.  Unlike most ESP chips, it does not include built-in wireless connectivity, allowing it to focus entirely on CPU performance, memory bandwidth, and peripheral capabilities. Wireless connectivity can instead be added using companion chips.
### ESP32-S3 Structure
The ESP32-S3 is a powerful microcontroller designed for AIoT (Artificial Intelligence of Things) applications. It integrates wireless connectivity, AI acceleration, and a rich set of peripherals on a single chip.

Understanding its internal structure helps us see how it processes data, communicates with devices, and connects to networks.

To understand how the ESP32-S3 works, we need to break down its core components:

<img src="./attachments/esp32s3pins.png" />



#### Microcontroller Core
The ESP32-S3 contains a dual-core Xtensa LX7 processor capable of running at speeds up to 240 MHz.   
This processor acts as the brain of the system. It executes the program instructions, processes data from sensors, performs calculations, and controls outputs such as displays, motors, and communication interfaces.

Because it has two processing cores, tasks can be distributed efficiently. For example, one core can manage communication or background processes while the other handles application logic or signal processing.
#### Memory Structure
The ESP32-S3 includes 512 KB of internal SRAM, which is used as working memory while the program is running.

In addition to the internal memory, the chip supports external high-speed SPI flash memory and PSRAM. These external memories allow developers to store larger programs, buffers, and datasets required by more complex applications such as image processing or AI workloads, Most ESP32 modules come with a massive 4 MB to 16 MB of flash memory.

#### Wi-Fi and Bluetooth Connectivity
One of the key features of the ESP32-S3 is its integrated wireless communication capabilities.

It includes built-in 2.4 GHz Wi-Fi (802.11 b/g/n) for network connectivity and Bluetooth 5 Low Energy (LE) for short-range wireless communication between devices.

Bluetooth LE supports modern features such as long-range communication using coded PHY, higher data throughput with 2 Mbps PHY, and extended advertising capabilities. These wireless technologies allow the ESP32-S3 to easily connect to smartphones, sensors, cloud services, and other smart devices.
#### AI Acceleration
The ESP32-S3 is designed with vector instruction support inside the processor. These instructions accelerate operations commonly used in signal processing and neural network computations.

This hardware acceleration allows developers to run lightweight machine learning models directly on the device. Libraries such as ESP-DSP and ESP-NN provide optimized implementations that make it easier to build AI-enabled applications such as voice recognition, gesture detection, and computer vision.

#### Input / Output Peripherals
The ESP32-S3 offers a rich set of peripherals that allow it to interact with a wide variety of electronic components.

It provides up to 45 programmable GPIO pins that can be configured for different functions. These pins can interface with sensors, displays, storage devices, and communication modules.

The chip also supports multiple communication interfaces including:
- **SPI**
- **I2C**
- **I2S**
- **UART**
- **PWM**
- **RMT**
- **SD/MMC host**

Additionally, 14 GPIO pins can function as capacitive touch inputs, making it possible to create touch-based human-machine interfaces.
#### Analog Functions
The ESP32-S3 includes an Analog-to-Digital Converter (ADC) that allows the system to read analog voltage signals from sensors such as temperature sensors, light sensors, or potentiometers.

The ADC converts these analog voltages into digital values that the microcontroller can process in software.
#### Ultra-Low-Power (ULP) Coprocessor
To support energy-efficient applications, the ESP32-S3 includes an ultra-low-power coprocessor.

This small processor can operate while the main cores are in sleep mode. It can monitor sensors, collect data, or trigger wake-up events while consuming very little power. This feature is particularly useful for battery-powered devices and long-term monitoring systems.

### ESP-IDF

To develop applications for the ESP32-S3 and other ESP chips, we use **ESP-IDF** (Espressif IoT Development Framework).  
ESP-IDF is the official development framework provided by Espressif Systems for programming ESP32-series microcontrollers. It is a powerful environment designed for professional and advanced embedded system development.

ESP-IDF provides the tools needed to write, compile, and upload firmware to ESP devices. It includes a set of libraries, drivers, and development tools that allow developers to access the hardware features of the microcontroller such as GPIO, Wi-Fi, Bluetooth, timers, and communication interfaces.

The framework can be downloaded from the official website and documentation:  
[https://docs.espressif.com/projects/esp-idf/en/latest/esp32/](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)

ESP-IDF also includes many example projects that help users understand how to work with different peripherals and features of the ESP chips. These examples demonstrate tasks such as connecting to Wi-Fi networks, reading sensor data, controlling displays, and using Bluetooth communication. In addition, the framework provides built-in component libraries that simplify working with hardware modules and allow developers to build complex IoT applications more efficiently.

## ESP32 Programming 

### Introduction
To program the ESP32 microcontroller using the ESP-IDF framework, we mainly use C and C++. The project structure typically contains a `ProjectName.c` file, which serves as the entry point of the application. It also includes build configuration files, which compile the source code into binary instructions that the microcontroller can execute.

he `ProjectName.c` file contains the `app_main()` function, which runs once when the **ESP32** board is powered on or reset. Inside this function, we place the instructions that we want the microcontroller to execute. If we need a set of instructions to run continuously, we can include a `while(1)` loop so that the code keeps executing repeatedly.

Before getting started, let's explore the key structure and syntax of the C language.
### Variables and Data Types
Variables in C can be thought of as containers or boxes used to store information that our program can use and manipulate. Each variable has a data type that tells the compiler what kind of data it will hold, such as numbers, text, or logical states.

To create a variable, we start with declaring the data type of the variable, then the variable name, and if it has a value, we assign the value to it using the `=` operator:
```c
type name = value;
```
ESP-IDF supports standard C data types:
#### `char`
Represents a single character, such as `'a'`, `'!'`, or `'$'`.
- Uses 1 byte of memory.
- Stored using ASCII encoding.
- Arithmetic operations on `char` actually operate on their ASCII values.

```c
char letter = 'A';  // ASCII value 65
```

#### `char` Arrays 
Represents a sequence of characters (text) like "Hello" or "ESP32". In C, strings are simply arrays of characters terminated by a null character (`\0`). Example:
```c
char message[] = "Hello ESP32";
```
#### `int`
Represents integer numbers , such as `5`, `-10`, or `0`.
- Because the ESP32 is a 32-bit microcontroller, `int` is **4 bytes**.
- Arithmetic operations on integers return integer values (e.g., `4 / 3` gives `1`).
```c
int count = 10;
```
Numbers can be written in different numeral systems:
- Decimal (base 10): normal numbers like 10, 25, 100.
- Binary (base 2): prefixed with `0b`.
- Octal (base 8): prefixed with `0`.
- Hexadecimal (base 16): prefixed with `0x`.
```c
int count = 10;      // decimal
int bin   = 0b1010;  // binary (10 in decimal)
int oct   = 012;     // octal (10 in decimal)
int hex   = 0xA;     // hexadecimal (10 in decimal)
```
#### `long` and `long long`
Used for integer numbers. On the ESP32, `long` is 4 bytes (same as `int`), while `long long` uses 8 bytes for massive numbers.
```c
long long milliseconds = 10000000000LL;
```
#### `float`
Represents decimal numbers with single-precision.
- Uses 4 bytes.
- Used for values like `3.14` or `-2.5`.

```c
float pi = 3.14f;
```
#### `double`
Represents decimal numbers with higher precision.
- Uses 8 bytes on the ESP32 for highly precise calculations.
```c
double precisePi = 3.1415926535;
```
#### `bool`
Represents a logical value, either true or false. To use this in C, you must include the `<stdbool.h>` library. Internally stored as `1` (true) or `0` (false). 
```c
#include <stdbool.h>
bool isLEDOn = true;
```
#### `void`
Represents no value. Used for functions that do not return anything. Example:
```c
void blinkLED() {  
  // This function does not return a value  
}
```
#### Fixed-Width Integer Types
In embedded programming with ESP-IDF, it is highly recommended to use fixed-width integers from `<stdint.h>` to ensure we know exactly how much memory a variable uses.
- `uint8_t`: An unsigned 1-byte integer (commonly used as a "byte").
- `int16_t`: A signed 2-byte integer.
- `uint32_t`: An unsigned 4-byte integer.
#### Arrays
Arrays are collections of variables of the same type stored together.
- Elements are accessed using indices, starting from `0`.
- Arrays can hold integers, characters, or other data types.

```c
int numbers[] = {4, 5, 7};           // int array  
char message[] = "hello";            // char array (string)  
char name[] = {'A', 'l', 'i', '\0'}; // char array manually terminated
```

### Data Types Table (ESP32 / 32-bit Architecture)

|Type|Memory|Value Range|
|---|---|---|
|`bool`|1 byte|`true` or `false`|
|`char`|1 byte|-128 to 127|
|`unsigned char`|1 byte|0 to 255|
|`short`|2 bytes|-32,768 to 32,767|
|`unsigned short`|2 bytes|0 to 65,535|
|`int`|4 bytes|-2,147,483,648 to 2,147,483,647|
|`unsigned int`|4 bytes|0 to 4,294,967,295|
|`long`|4 bytes|-2,147,483,648 to 2,147,483,647|
|`unsigned long`|4 bytes|0 to 4,294,967,295|
|`long long`|8 bytes|-(2^63) to (2^63)-1|
|`float`|4 bytes|~ ±3.4 × 10³⁸|
|`double`|8 bytes|~ ±1.7 × 10³⁰⁸|
|`void`|0 bytes|no value|
|`int8_t`|1 byte|-128 → 127|
|`uint8_t`|1 byte|0 → 255|
|`int16_t`|2 bytes|-32,768 → 32,767|
|`uint16_t`|2 bytes|0 → 65,535|
|`int32_t`|4 bytes|-2,147,483,648 → 2,147,483,647|
|`uint32_t`|4 bytes|0 → 4,294,967,295|
|`int64_t`|8 bytes|very large|
|`uint64_t`|8 bytes|very large|

### Constants
A constant is a variable whose value cannot change while the program runs. We use uppercase letters for constants to make them easy to identify in code. There are two ways to define constants:
- Using the `const` keyword: `const float GRAVITY = 9.81;`
- Using the `#define` directive: `#define PI 3.14159`


`#define` is a preprocessor directive that replaces the name with a value before compilation and has no type checking, while `const` creates a typed variable whose value cannot change, allowing the compiler to enforce type safety.

### Arithmetic Operators
Arithmetic operators are used to perform mathematical operations. They allow calculations such as addition, subtraction, multiplication, and division.

- **Addition (+):** Adds two operands. When used with char types, it adds their ASCII values and returns the resulting character.
- **Subtraction (-):** Subtracts the second operand from the first. Similar to addition, it operates on ASCII values when used with char types.
- **Multiplication (*):** Multiplies two operands. This operator cannot be directly used with char types.
- **Division (/):**
    - For float or double types, it performs regular division and returns the exact result.
    - For int or long types, it performs integer division, which discards any fractional part of the result. It does not round the result.
- **Modulo (%):** Returns the remainder of the division between two operands. It is only applicable for int and long types.
### Compound Operators
Compound operators combine an arithmetic operation with assignment in a single step. They make the code shorter, clearer, and easier to read.

- **`+=` (Compound addition):**  Adds a value to a variable and assigns the result back to the same variable.  `a += 5;` same as ``x = x + 5;``  
- **`-=` (Compound subtraction):**  Subtracts a value from a variable.  ``x -= 2;``   same as ``x = x - 2;``    
- **`*=` (Compound multiplication):**  Multiplies a variable by a value. ``x *= 4;``   same as ``x = x * 4;``  
- **`/=` (Compound division):**  Divides a variable by a value.  ``x /= 2;``  same as ``x = x / 2;``  
- **`%=` (Compound remainder):**  Stores the remainder of division.  ``x %= 3;``   same as ``x = x % 3;`` 

#### Increment and Decrement Operators
These operators are special compound operators used to increase or decrease a variable by 1. They are very common in loops and counters.

- **`++` (Increment):**  Increases a variable by one.  ``i++;``   same as ``i = i + 1;``    
- **`--` (Decrement):**  Decreases a variable by one.  ``i--;``  same as ``i = i - 1;`` 

These operators can be used in two forms:
- **`++i` (pre-increment)**: increment first, then use the value.
- **`i++` (post-increment)**: use the value first, then increment.

### Comparison Operators
Comparison operators compare two values and produce a boolean result (`true` or `false`, or `1` / `0` in standard C).

|Operator|Meaning|
|---|---|
|==|Equal to|
|!=|Not equal|
|>|Greater than|
|<|Less than|
|>=|Greater or equal|
|<=|Less or equal|

### Boolean Operators
Boolean operator help us to  combine multiple conditions into more complex expressions. The result of a logical operation is also a boolean value (true or false).
- **|| (OR):** Returns **true** if at least one of the conditions connected by `||` is true.
- **&& (AND):**  Returns **true** only if all the conditions connected by `&&` are true.
- **! (NOT):**  Reverses the logical state of the condition.

| A      | B      | A && B  | A \|\| B  | !A     |
| ------ | ------ | ------- | --------- | ------ |
| true   | true   | true    | true      | false  |
| true   | false  | false   | true      | false  |
| false  | true   | false   | true      | true   |
| false  | false  | false   | false     | true   |


### Conditional Statements
#### Single Condition with `if`
Sometimes we need to run an instruction only when a condition is true. If the condition is false, the System simply skips that block and continues running the rest of the program.
```c
if (condition){
instructions
}
```
#### Alternative Path with `if-else`
We can also provide an alternative path. If the condition is true, one block runs; otherwise, the `else` block runs.
```c
if (condition){
instructions to run if condition valide
}else{
instructions to run if condition not valide
}
```
#### Multiple Conditions with `else if`
When there are more than two possible outcomes, we can chain conditions using `else if`. C checks them in order and executes the first one that is true.
```c
if (condition 1){
instructions to run if condition 1 is valide
}else if(condition 2){
instructions to run if condition 2 is valide
}else{
instructions to run if non of the conditions is valide
}
```
#### Ternary Operator
In C, we can use the ternary operator `? :`, which is a shorthand form of the if–else statement used to assign a value to a variable based on a condition. It is useful for short and simple decisions, especially when assigning values.
```c
status = condition ? "value if true" : "value if false";
```
#### The `switch` `case`Statement
When checking a value against many options, We can use the `switch` statement. It makes the code cleaner compared to many `else if` conditions.
```c
switch (variable){
    case value1:
        instruction that run if variable == value1
    break;
    case value2:
        instruction that run if variable == value2
    break;
    case value3:
        instruction that run if variable == value3
    break;
    case value4:
        instruction that run if variable == value4
    break;
    default:
        instruction that run if variable have other value then we provided
    break;
}
```
### Loops
Loops help us to repeat an action multiple times. there is three type of loops
#### The `for` Loop
A `for` loop is used when we know how many times we want to repeat a block of code. It is very common in Esp32, especially for controlling LEDs, sensors, or repeating tasks a fixed number of times.
```c
for (initialization;condition; increment/decrement){
    instructions we want to repeat
}
```
#### The `while` Loop
A `while` loop repeats as long as a condition is true. It is useful when we don’t know how many times the loop will run.
```c
while (condition){
instructions we want to repeat
}
```
#### The `do-while` Loop
A `do-while` loop is similar to `while`, but it always runs at least once, even if the condition is false.
```c
do{
    instructions we want to repeat
}while (condition);
```
#### Break and Continue :
We can enhance the control of our loops by using the keywords `break` and `continue`:
- **`break`**: The `break` statement is used to terminate and exit a loop immediately when a specific condition is met.
- **`continue`**: The `continue` statement is used to skip the current iteration of the loop and proceed with the next one, effectively ignoring the instructions for that iteration when a specific condition is true.

### Comments :
Comments are lines of code that the compiler will ignore. They are used to add explanatory notes and improve the readability of our code. There are two ways to create comments:
- **Single-line comments:** Begin with ``//`` and continue until the end of the current line. All text following ``//`` on the same line is ignored by the compiler.
- **Multi-line comments:** Begin with ``/*`` and end with ``*/``. Any text between these two symbols, including multiple lines, will be ignored by the compiler.

## Creating Our First Projects
Now that we understand what the ESP32‑S3 microcontroller is and have reviewed the basic concepts of the C, we can begin writing a simple program for the ESP32-S3.

A traditional first project in embedded systems is a blinking LED. This project helps us verify that:
- The development environment works correctly
- The code compiles successfully
- The board can control its GPIO pins

Before writing any code, we need to set up the project folder structure used by the ESP‑IDF.
### Setting Up the Project Folder
First, create a folder that will contain all the files related to our project.  ESP-IDF projects follow a standard directory structure that allows the build system to compile and organize our code properly.

A minimal ESP-IDF project structure looks like this:
```
my-new-project/ 
├── CMakeLists.txt (Top)
└── main/ 
	├── CMakeLists.txt 
	└── main.c
```
- `my-new-project` represents the root folder of our project It contains configuration files and subfolders that define the entire project.
- `CMakeLists.txt` defines global build settings for the project, we set in it where ESP-IDF build system is located, which components belong to the project and how to organize the build process.

```txt
# The minimum version of CMake required 
cmake_minimum_required(VERSION 3.16) 

# Include the ESP-IDF build system include($ENV{IDF_PATH}/tools/cmake/project.cmake) 

# Our project name 
project(my-new-project)
```
- `main/` In ESP-IDF, projects are organized into into components. which are small building bock that handel specific tasks,The `main` folder is the default component that contains our main application code. In small projects, all code can remain inside this folder.  
- `main/CMakeLists.txt` This component level build configuration file it tells the build system which source files belong to this component.
```
idf_component_register(SRCS "main.c"  
INCLUDE_DIRS ".")
```
We to set two main things in this file
	- `SRCS` lists the source files to compile
	- `INCLUDE_DIRS` specifies directories containing header files

- `main/main.c` This file contains the actual program code that will run on the ESP32-S3.

Lets apply that and create `blink` folder and inside it, we add the files we discussed above.

### Creating The Circuit
Now that we have set up our project folder, let's create the circuit, We will need an LED (Light Emitting Diode). An LED allows current to flow in only one direction and has two legs:
- **Longer leg (Anode):** connects to the positive side (signal or power).
- **Shorter leg (Cathode):** connects to **ground (GND)**.

We will also need a 220-ohm resistor. The resistor limits the current flowing through the LED to prevent it from burning out.     
To control the LED and make it blink, we will use a digital GPIO pin on the ESP32. For this example, we will use GPIO 2.

Now lets build our circuit: 
1. Connect the long leg (anode) of the LED to GPIO 2 through the 220-ohm resistor.
2. Connect the short leg (cathode) of the LED to a GND pin on the ESP32.

<img src="./attachments/circuit.png"/>


### Creating the Program
Now let’s program the ESP32 board. We go to `main/main.c` file, and first we start by importing the library that we will need
```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
```
**`#include <stdio.h>`**  is the Standard Input/Output library from C. It allows us to use functions such as `printf()` to print messages to the serial monitor. 

**`#include "freertos/FreeRTOS.h"`** This header includes the FreeRTOS core definitions used by the ESP-IDF framework. FreeRTOS is the real-time operating system that runs on the ESP32 and manages tasks, timing, and system resources.

**`#include "freertos/task.h"`**  This library provides functions for creating and managing tasks in FreeRTOS. It also includes useful functions such as `vTaskDelay()` that allow us to pause a task for a specific amount of time .

**`#include "driver/gpio.h"`**  This header gives us access to the GPIO (General Purpose Input/Output) driver of the ESP32. It allows us to configure pins as inputs or outputs and control them, which is how we will turn the LED on and off using GPIO 2.

Now we create our `app_main` function,  we start by setting the mode of the pin,after that 
```c
void app_main(void){
	gpio_reset_pin(2);
	gpio_set_direction(2, GPIO_MODE_OUTPUT);
```
After that, we create an infinite `while` loop. Inside this loop, we control the LED by sending signals to GPIO 2.   
To turn the LED on and off, we use the function:`gpio_set_level(PIN, signal);` which sets the electrical level of the specified GPIO pin.
- **`PIN`** represents the GPIO number we want to control (in our case, **GPIO 2**).
- **`signal`** determines the output level:
    - `1`  → turns the LED **on**
    - `0`  → turns the LED **off**

To create the blinking effect, we add a delay between turning the LED on and off using: `vTaskDelay(1000 / portTICK_PERIOD_MS);` which the task for a specific amount of time.
- **`1000`** represents the delay time in **milliseconds (1 second)**.
- **`portTICK_PERIOD_MS`** converts milliseconds into **FreeRTOS system ticks**, which is the time unit used by the operating system.

```c
    while (1) {
        gpio_set_level(2, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(2, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
```
### Executing The Program
Now let's execute and flash the program into our microcontroller.
#### Connect and Set Target
We start by plugging in the microcontroller via USB. After that, we select the target microcontroller (the ESP32-S3) so the compiler knows which chip architecture to use:
```shell
idf.py set-target esp32s3
```
### Identify the Port
Next, we check on what port the microcontroller is connected to. We can find this by scanning the connected hardware:
```shell
esptool.py chip_id
```
On Windows, this will look like `COM3`; on Linux/macOS, it will look like `/dev/ttyUSB0` or `/dev/cu.usbmodem...`.
#### Build the Project
After that we compile our code into a machine-readable format. This step checks for errors in our C code:
```shell
idf.py build
```
#### Flash and Monitor
Finally, we flash our program.
```
idf.py -p PORT flash monitor
```
Replace `PORT` with the ID you found in step 2:
 