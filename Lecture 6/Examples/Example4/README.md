# Working with Mutexes

This example demonstrates how to protect shared resources in a FreeRTOS application running on an ESP32-S3 using **Mutexes**. The application is organized into separate software components, with two independent tasks that both write messages to the serial terminal. Since the terminal is a shared resource, a mutex is used to ensure that only one task can access it at a time, preventing conflicting operations and maintaining consistent output.

The project begins by creating a global mutex handle of type `SemaphoreHandle_t`. The mutex itself is created in `app_main()` using `xSemaphoreCreateMutex()`. After verifying that the mutex was successfully created, two independent tasks are launched.

The **Task 1 Component** contains `print_task_1()`, which periodically writes a multi-line message to the serial terminal. Before accessing the terminal, the task acquires ownership of the mutex using `xSemaphoreTake()`. If another task already owns the mutex, the task automatically enters the **Blocked** state and waits until the resource becomes available. Once the terminal has been acquired, the task prints its messages, releases the mutex with `xSemaphoreGive()`, and then delays before repeating the process.

The **Task 2 Component** contains `print_task_2()`, which performs a similar operation by printing a different message to the terminal. Like the first task, it must successfully acquire the mutex before calling `printf()`. After completing its output, the task releases the mutex, allowing other waiting tasks to safely access the shared resource.

The mutex is shared between both task components through a global `SemaphoreHandle_t` variable. Rather than communicating with each other directly, the tasks synchronize access to the terminal by requesting ownership of the mutex whenever they need to use the shared resource. The FreeRTOS scheduler automatically blocks any task that attempts to acquire a mutex that is already locked, ensuring that only one task can access the terminal at any moment.

