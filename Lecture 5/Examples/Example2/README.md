# ESP32-to-ESP32 I²C Communication

This example demonstrates how to establish communication between two ESP32-S3 development boards using the I²C protocol. One ESP32-S3 is configured as the I²C master, while the second ESP32-S3 operates as an I²C slave.

A push button is connected to the master ESP32-S3, and an LED is connected to the slave ESP32-S3. Whenever the button is pressed, the master toggles an internal state and transmits either the message "ON" or "OFF" to the slave using I²C communication. The slave receives the message through an event callback, interprets the received data, and updates the LED state accordingly. Receiving "ON" turns the LED on, while receiving "OFF" turns it off.

The two ESP32-S3 boards communicate through shared SDA and SCL lines connected to GPIO 8 and GPIO 9 respectively. Pull-up resistors are used on both bus lines to ensure proper I²C operation. The master configures the I²C bus, registers the slave device address, and transmits commands, while the slave listens for incoming data and processes it through the ESP-IDF I²C slave driver callbacks.

This project demonstrates the complete I²C communication workflow, including master and slave configuration, device addressing, data transmission, event-driven data reception, and using received messages to control external hardware.


<img src="../../attachments/i2c_circuit.png" />