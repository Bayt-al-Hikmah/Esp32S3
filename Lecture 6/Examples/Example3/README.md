# FreeRTOS Inter-Task Communication

This example demonstrates how to safely exchange data between multiple FreeRTOS tasks on an ESP32-S3 using **Queues**. The application is organized into separate software components, with one component responsible for reading data from an LDR (Light Dependent Resistor) sensor and another responsible for controlling an LED. 

The project begins by creating a global queue handle of type `QueueHandle_t`. The queue itself is created in `app_main()` using `xQueueCreate()`, where it is configured to store up to five integer values representing LDR sensor readings. Once the queue has been successfully created, two independent tasks are launched.

The **Sensor Component** contains the `sensor_task()`, which initializes the ADC peripheral, continuously reads the analog value from the LDR sensor, and sends each reading to the queue using `xQueueSend()`. If the queue is full, the function waits for up to 100 milliseconds before timing out. After sending a value, the task calls `vTaskDelay()`, allowing the scheduler to execute other ready tasks.

The **Light Component** contains the `light_task()`, which configures GPIO 7 as an output and continuously waits for incoming sensor readings using `xQueueReceive()`. By specifying `portMAX_DELAY`, the task automatically enters the **Blocked** state whenever the queue is empty, consuming no CPU time while waiting for new data. When a sensor value is received, the task compares it against a predefined threshold. If the measured light level exceeds the threshold, the LED is turned on; otherwise, it is turned off.

Inside `app_main()`, the queue is created before any tasks are started. Both the sensor and light tasks receive access to the same queue through the shared global queue handle, allowing them to communicate without directly sharing sensor data. 

<img src="../../attachments/ldr_circuit.png" />
