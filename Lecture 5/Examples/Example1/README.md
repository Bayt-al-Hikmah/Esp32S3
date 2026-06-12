# ESP32-to-ESP32 UART Communication

This example demonstrates how to establish serial communication between two ESP32-S3 development boards using the UART protocol. One ESP32-S3 acts as a transmitter connected to a push button, while the second ESP32-S3 acts as a receiver connected to an LED.

When the push button is pressed on the first ESP32-S3, the board toggles an internal state and sends either the text message "ON" or "OFF" through UART. The second ESP32-S3 continuously listens for incoming UART data, interprets the received message, and updates the LED state accordingly. Receiving "ON" turns the LED on, while receiving "OFF" turns it off.

The project uses UART1 configured at 115200 baud with an 8-N-1 communication format (8 data bits, no parity, and 1 stop bit). Communication is established through GPIO 17 (TX) and GPIO 18 (RX) on both boards. This example demonstrates the complete UART workflow, including peripheral configuration, data transmission, data reception, and using received serial commands to control external hardware.

<img src="../../attachments/circuit1.png" />