# FreeRTOS Multi-Task Counter with NVS
This example demonstrates how to combine FreeRTOS tasks with Non-Volatile Storage (NVS) to build a simple button counter on an ESP32-S3. Two independent tasks are created, with one task responsible for incrementing the counter and the other responsible for resetting it. The counter value is stored in NVS, allowing it to persist even after the board is powered off or restarted.

The project uses two push buttons connected to the ESP32-S3. The Count Up button on **GPIO 4** increases the counter each time it is pressed, while the **Reset** button on **GPIO 5** clears the counter and sets its value back to zero. Whenever the counter changes, the updated value is immediately written to NVS, ensuring that the most recent count is preserved across reboots.


<img src="../../attachments/circuit_nvs.png" />
