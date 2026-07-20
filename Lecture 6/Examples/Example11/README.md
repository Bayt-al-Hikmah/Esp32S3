# FreeRTOS Task Stack Monitoring

This example demonstrates how task memory is managed in FreeRTOS and how to monitor stack usage on an ESP32-S3. Unlike a Bare-Metal application, where the entire program shares a single main stack, every FreeRTOS task has its own dedicated stack. Understanding how task stacks are allocated and monitored is essential for preventing stack overflows and building reliable embedded applications.

To monitor stack usage, the task periodically calls `uxTaskGetStackHighWaterMark()`. This function returns the smallest amount of unused stack space that has remained since the task started running, often referred to as the **Stack High Water Mark**. A large value indicates that plenty of stack space is still available, while a value approaching zero suggests the task is at risk of a stack overflow and its allocated stack size should be increased.

The task executes continuously inside an infinite loop, printing its current Stack High Water Mark every two seconds before calling `vTaskDelay()`. During this delay, the task enters the **Blocked** state, allowing the FreeRTOS scheduler to execute other ready tasks. 
