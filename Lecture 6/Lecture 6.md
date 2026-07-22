## Objectives
- Exploring Real-Time Operating Systems (RTOS)
- Exploring Memory Structure, Usage, and Manipulation on the ESP32-S3
## Real-Time Operating Systems (RTOS)
When building simple embedded projects, we usually write our code inside a single infinite loop, often called a "Super Loop" or "Bare-Metal" programming. In this approach, the microcontroller executes one instruction after another. If a specific function takes a long time to complete or if we use a blocking function, the entire processor stops, and no other code can run until that function finishes.    
As embedded systems become more complex by combining Wi-Fi, sensor reading, display updates, and user input, the Super Loop approach begins to becomes inefficient. The system becomes unresponsive and difficult to manage.  
To overcome this limitation, a Real-Time Operating System (RTOS) is used. An RTOS acts as a highly efficient manager for the microcontroller. Instead of one massive loop, we break our program down into smaller, independent programs called tasks. The RTOS rapidly switches the processor's attention between these tasks based on timing and priority. This rapid switching creates the illusion of simultaneous execution, making the system highly responsive and deterministic.
### FreeRTOS and the ESP32-S3
The ESP-IDF framework uses FreeRTOS as its core operating system. FreeRTOS is the industry standard for embedded systems. However, traditional FreeRTOS deployments are commonly single-core. Because the ESP32-S3 has a dual-core processor, Espressif heavily modified FreeRTOS to support SMP (Symmetric Multiprocessing).

This means the ESP-IDF version of FreeRTOS allows us to not only split our program into tasks but also explicitly dictate whether a task runs on Core 0, which is commonly used by system and communication tasks such as Wi-Fi/Bluetooth or Core 1 which is reserved for application code.
### Scheduler Behavior
At the center of FreeRTOS lies the scheduler, the component responsible for deciding which task gets access to the CPU at any given moment. The scheduler operates using a periodic signal known as the tick interrupt. A hardware timer inside the microcontroller generates this interrupt at a fixed frequency, commonly every 1 millisecond (1 kHz) in ESP-IDF systems. Each time this interrupt occurs, FreeRTOS briefly pauses normal execution and reevaluates the state of all tasks in the system.

This periodic reevaluation is what gives FreeRTOS its “real-time” behavior. Instead of allowing a single program flow to run endlessly, the operating system constantly checks whether another task should take control of the processor. Tasks can be in different states such as _Running_, _Ready_, _Blocked_, or _Suspended_. The scheduler’s job is to examine these states and choose the highest-priority task that is ready to execute.

FreeRTOS uses a preemptive scheduling model, which means that task priority directly influences CPU access. If a higher-priority task becomes ready while a lower-priority task is currently running, the scheduler immediately interrupts the lower-priority task and switches execution to the higher-priority one. This process is called preemption. 

The scheduler also manages situations where multiple tasks share the same priority level. In this case, FreeRTOS typically uses a technique called **time slicing**. Rather than allowing one task to monopolize the CPU, the scheduler divides processor time into small slices based on the system tick. One task may run for a single tick, after which another task of equal priority gets a turn. This alternating pattern continues as long as both tasks remain ready to run. 

An important detail is that context switching happens very quickly. FreeRTOS saves the current task’s execution context, including CPU registers and stack information, before restoring the context of the next task. Because this process is lightweight, the operating system can switch between tasks rapidly enough that multitasking appears simultaneous, even on a single-core processor.
### Tasks
A task is the most fundamental building block of FreeRTOS. We can think of a task as an independent program with its own infinite loop, its own priority, and its own allocated memory (called a stack).

Tasks in FreeRTOS operate in different states:
- **Running:** The task is currently executing on the CPU core.
- **Ready:** The task is ready to run but is waiting because a higher-priority task is currently using the CPU.
- **Blocked:** The task is waiting for something to happen (like a timer to expire, or waiting for data from a sensor). While blocked, it consumes **zero** CPU time.
- **Suspended:** The task is explicitly paused by the programmer and will not run again until explicitly resumed.
#### Task Priorities
Every task is assigned a priority from 0 to ``configMAX_PRIORITIES - 1``. In ESP-IDF, the maximum value is usually 24. Higher numbers represent higher priority. The FreeRTOS scheduler strictly obeys priority: it will always pause a lower-priority task if a higher-priority task is ready to run.
#### Programming Tasks in ESP-IDF
Let’s build a simple project to demonstrate tasks. We want to blink two LEDs at completely different frequencies. We can implement this using a super loop with delays, and it works, but this approach produces blocking code. If the frequencies are unrelated, we may run into problems. With FreeRTOS, we can assign each LED its own individual task.

First, we define our task functions. Every task in FreeRTOS must return `void`, take a `void *` parameter, and contain an infinite loop. Crucially, we must use `vTaskDelay()` instead of a standard delay. `vTaskDelay()` puts the task into the **Blocked** state, freeing up the CPU core to run other tasks.
```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_1 4
#define LED_2 5

// Task 1: Blinks LED 1 every 500ms
void blink_task_1(void *pvParameters) {
    gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
    
    while (1) {
        gpio_set_level(LED_1, 1);
        vTaskDelay(pdMS_TO_TICKS(500)); // Block for 500ms
        gpio_set_level(LED_1, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Task 2: Blinks LED 2 every 1000ms
void blink_task_2(void *pvParameters) {
    gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
    
    while (1) {
        gpio_set_level(LED_2, 1);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Block for 1000ms
        gpio_set_level(LED_2, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```
Now, inside `app_main()`, we create these tasks using `xTaskCreatePinnedToCore`. This function requires:
1. The function name of the task.
2. A human-readable string name for debugging.
3. The stack size in bytes, which determines how much memory the task needs.
4. Parameters to pass to the task; here, we pass ``NULL``.
5. The priority level; we use ``1`` for both tasks.
6. A task handle; we pass ``NULL`` because we do not need to reference it later..
7. The core ID `0` or `1`, or `tskNO_AFFINITY` to let the RTOS choose.

```c
void app_main(void) {
    // Create Task 1 on Core 1
    xTaskCreatePinnedToCore(blink_task_1, "Task 1", 2048, NULL, 1, NULL, 1);
    
    // Create Task 2 on Core 1
    xTaskCreatePinnedToCore(blink_task_2, "Task 2", 2048, NULL, 1, NULL, 1);
}
```
#### Task Management: Suspend, Resume, and Delete
We can do more than just create and run tasks. We can also control their states and lifecycle. To do this, we need to capture the Task Handle when we create it.
- **`vTaskSuspend(TaskHandle_t xTaskToSuspend)`**: Pauses a task. It goes into the Suspended state and ignores all RTOS events until resumed.
- **`vTaskResume(TaskHandle_t xTaskToResume)`**: Brings a task out of the Suspended state and back to Ready.
- **`vTaskDelete(TaskHandle_t xTaskToDelete)`**: Completely destroys a task and frees its allocated stack memory. (To "restart" a task, you must delete it and call `xTaskCreate` again).

```c
TaskHandle_t myTaskHandle = NULL;

void app_main(void) {
    // 1. Create the task and save its handle
    xTaskCreatePinnedToCore(blink_task_1, "Task 1", 2048, NULL, 1, &myTaskHandle, 1);
    
    // 2. Suspend the task (Stop it)
    vTaskSuspend(myTaskHandle);
    // 3. Resume the task 
    vTaskResume(myTaskHandle);
    // 4. Delete the task (Passing NULL deletes the task that calls it)
    vTaskDelete(myTaskHandle); 
}
```
### Inter-Task Communication: Queues
Once we have multiple tasks running, they inevitably need to share data. For example, a "Sensor Task" reads temperature data, and a "Display Task" prints it to an OLED screen.

We can pass this data using Global Variables. In an RTOS, global variables are dangerous. If the Sensor Task is halfway through updating a global variable, and the RTOS suddenly switches to the Display Task, the Display Task might read corrupted or incomplete data.

To solve this, FreeRTOS provides Queues. A Queue is a safe, First-In-First-Out (FIFO) pipeline between tasks.
- The sending task pushes data to the back of the queue.
- The receiving task pulls data from the front of the queue.
- If the queue is empty, the receiving task automatically enters the Blocked state until data arrives, wasting zero CPU time.
#### Programming Queues
To work with queues, we first need to create a global variable of type `QueueHandle_t`. After that, we can manipulate the queue using three main functions:

- `xQueueCreate()` creates a queue object. It takes two arguments: the length of the queue (the maximum number of elements it can hold) and the size of each element stored in the queue.
- `xQueueSend()` sends new data to the queue. It takes three arguments: the global queue variable, the data being added, and the timeout value that specifies how long the function should wait if the queue is full.
- `xQueueReceive()` reads and retrieves data from the queue. Like the previous function, it takes three arguments: the queue handle, a pointer to where the received data should be stored, and the timeout value that specifies how long the function should wait for data.

Let’s recreate the automated lighting system that we built in Lecture 3 using queues and tasks. First, we make our circuit.

<img src="./attachments/ldr_circuit.png" />

Now we can create our program. We start by importing the required libraries and creating the global queue variable.
```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_adc/adc_oneshot.h"
#include "driver/gpio.h"

QueueHandle_t data_queue;
```
After that, we create two functions that represent our tasks:
- The **producer task** reads data from the LDR sensor and sends it to the queue.
- The **consumer task** receives data from the queue and controls the LED based on the received value.
```C
    
void producer_task(void *pvParameters) {

    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, .atten = ADC_ATTEN_DB_12
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_4, &config);

    int ldrValue = 0;
    while (1) {
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_4, &ldrValue);
        xQueueSend(data_queue, &ldrValue , pdMS_TO_TICKS(100));
        vTaskDelay(pdMS_TO_TICKS(500)); 
    }
}

void consumer_task(void *pvParameters) {
    int received_data = 0;
    while (1) {
        // Wait FOREVER (portMAX_DELAY) for data to arrive in the queue
       xQueueReceive(data_queue, &received_data, portMAX_DELAY);
       if (received_data > 1600) {
            gpio_set_level(7, 1); 
        } else {
            gpio_set_level(7, 0); 
        }
    
    }
}
```
Finally, inside `app_main()`, we initialize GPIO 7 as an output pin, create the queue, and then launch both tasks.
```c
void app_main(void) {
    gpio_reset_pin(7);
    gpio_set_direction(7, GPIO_MODE_OUTPUT);
    
    data_queue = xQueueCreate(5, sizeof(int));

    if (data_queue != NULL) {
        xTaskCreatePinnedToCore(producer_task, "Producer", 2048, NULL, 1, NULL, 1);
        xTaskCreatePinnedToCore(consumer_task, "Consumer", 2048, NULL, 1, NULL, 1);
    }
}
```

### Resource Management: Mutexes
Queues are great for moving data, but what if two tasks need to access the exact same physical hardware resource?   
Imagine two tasks both trying to use the I²C bus or the UART terminal at the exact same millisecond. The data sent to the terminal will become garbled, mixing the outputs of both tasks together. This is known as a Race Condition.

To prevent this, FreeRTOS provides a Mutex (Mutual Exclusion). A Mutex acts like a physical key to a locked room.
1. Before a task can use a shared resource, it must Take the Mutex.
2. If another task tries to use the resource, it sees the Mutex is gone and immediately enters the Blocked state, waiting at the door.
3. When the first task finishes, it "Gives" the Mutex back.
4. The waiting task then grabs the Mutex, locks the door, and proceeds.
#### Programming a Mutex
To work with mutexes, we first need to create a global variable of type `SemaphoreHandle_t`. After that, we can manipulate the mutex using two main functions:

- `xSemaphoreCreateMutex()` Creates a mutex object and returns a handle to it.
- `xSemaphoreTake()` Attempts to lock the mutex before accessing a shared resource. It takes two arguments: the mutex handle and a timeout value that specifies how long the task should wait if the mutex is already locked.
- `xSemaphoreGive()` Releases the mutex after the shared resource is no longer being used, allowing other tasks to access it.

Let’s create an example where two tasks try to write messages to the terminal at the same time. Since the terminal is a shared resource, we will use a mutex to make sure only one task can write at a time.

We start by importing the required libraries and creating the global mutex variable.
```c
#include "freertos/FreeRTOS.h"  
#include "freertos/task.h"  
#include "freertos/semphr.h"  
#include <stdio.h>  
  
SemaphoreHandle_t terminal_mutex;
```
After that, we create two tasks:
- The first task prints a long message to the terminal.
- The second task also prints a different message.
- Both tasks must lock the mutex before using `printf()`.
```C
void print_task_1(void *pvParameters) {  
  
	while (1) {    
		xSemaphoreTake(terminal_mutex, portMAX_DELAY);  
		printf("Task 1 is writing a long message...\n");  
		vTaskDelay(pdMS_TO_TICKS(100));  
		printf("Task 1 finished writing.\n");  
		xSemaphoreGive(terminal_mutex);  
		vTaskDelay(pdMS_TO_TICKS(500));  
	}  
}  
  
void print_task_2(void *pvParameters) {  
	
	while (1) {  
		xSemaphoreTake(terminal_mutex, portMAX_DELAY);  
		printf("Task 2 is writing something different...\n");  
		vTaskDelay(pdMS_TO_TICKS(100));  
		printf("Task 2 finished writing.\n");  
		xSemaphoreGive(terminal_mutex);  
		vTaskDelay(pdMS_TO_TICKS(300));  
	}  
}
```
Finally, inside `app_main()`, we create the mutex and then launch both tasks.
```c
void app_main(void) {    // Create the mutex    
	terminal_mutex = xSemaphoreCreateMutex();    
	if (terminal_mutex != NULL) {        
		xTaskCreatePinnedToCore(print_task_1, "Printer1", 2048, NULL,1, NULL, 1);        
		xTaskCreatePinnedToCore(print_task_2, "Printer2", 2048, NULL, 1, NULL, 1); 
	}
}
```
Without the mutex, both tasks may try to use `printf()` at the same time, causing corrupted or mixed terminal output. By protecting the terminal with a mutex, only one task can access it at a time. This becomes more powerful when working with variables and data.
#### Priority Inversion
A common RTOS issue when using mutexes is **priority inversion**. It occurs when three tasks with different priorities interact:
- A **Low-priority** task acquires a mutex.
- A **High-priority** task later needs the same mutex, so it becomes blocked while waiting for it.
- Meanwhile, a **Medium-priority** task becomes ready to run.

Because the Medium-priority task has a higher priority than the low-priority task, it preempts the low-priority task and takes the CPU. The result is that the High-priority task remains blocked, waiting for the mutex, while the low-priority task cannot run long enough to release it.

In other words, the High-priority task is indirectly delayed by the Medium-priority task, even though the Medium-priority task does not use the mutex at all.

This kind of bug can be extremely difficult to debug and may cause serious timing problems in real-time systems.

To solve this problem, FreeRTOS mutexes implement priority inheritance. When a High-priority task blocks while waiting for a mutex owned by a low-priority task, the RTOS temporarily boosts the low-priority task to the High task’s priority level. This allows the low-priority task to run immediately, finish its critical section, release the mutex, and then return to its original priority.

### Signaling and Synchronization: Binary Semaphores
While mutexes are designed for resource protection (locking a door), a binary semaphore is mainly used for signaling,and synchronization between tasks or between an interrupt and a task. Unlike a mutex, it is not about ownership of a resource, but about notifying that an event has occurred. It behaves like a simple flag that can be either “available” (1) or “not available” (0). One part of the system “gives” the semaphore to send a signal, and another part “takes” it to wait for that signal.  

Binary semaphores are most commonly used to handle Interrupt Service Routines (ISRs). In embedded systems, we want our ISRs to be as fast as possible. If a button is pressed or a packet arrives, we shouldn't do heavy processing inside the interrupt itself, as this blocks the entire processor.

Instead, we use a technique called deferred interrupt processing:
1. The hardware triggers an Interrupt.
2. The ISR does the bare minimum and "Gives" a Binary Semaphore.
3. A Task that was "Taking" that semaphore wakes up and performs the heavy work.

#### Programming Binary Semaphores
To use a binary semaphore, we use the following functions:
- **`xSemaphoreCreateBinary()`**: This function initializes the semaphore object, but it is important to understand its initial state. Unlike a simple flag that might start as “available,” a newly created binary semaphore starts in the empty (not available) state. This means that the first `Take` operation will block until another part of the system explicitly gives the signal.

- **`xSemaphoreGiveFromISR()`**: This function sends a signal from within an Interrupt Service Routine (ISR), Its role is simply to notify the system that an event has occurred, without doing any heavy processing.
- **`xSemaphoreTake()`**: Used by the task to wait for the signal, A task calling this function will enter a blocked state until the semaphore is given. Once the ISR (or another task) issues the signal, the waiting task is immediately unblocked and resumes execution to handle the event.


Let’s see how these functions work by creating a simple project that toggles an LED when the BOOT button is pressed.

We start by including the required libraries and defining our constants. And declare a variable `xGuiSemaphore`, which will be used as a binary semaphore to synchronize the button interrupt with a task.
```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

#define BUTTON_GPIO 0
#define LED_GPIO    7

SemaphoreHandle_t xGuiSemaphore;
```
After that, we define the Interrupt Service Routine (ISR), which executes automatically whenever the button interrupt occurs. The function is placed in IRAM using ``IRAM_ATTR`` so it can execute quickly and reliably during interrupts.

Inside the ISR, we declare a variable named ``xHigherPriorityTaskWoken`` and initialize it to ``pdFALSE``. This variable is used by FreeRTOS to indicate whether giving the semaphore caused a higher-priority task to leave the Blocked state and become ready to run.

The function ``xSemaphoreGiveFromISR()`` gives the semaphore in an ISR-safe manner. If a higher-priority task was waiting for this semaphore, the function updates ``xHigherPriorityTaskWoken`` to ``pdTRUE``.

Finally, ``portYIELD_FROM_ISR()`` checks this flag and requests an immediate context switch before exiting the interrupt. This allows the awakened higher-priority task to run immediately instead of waiting for the next scheduler tick.
```c
static void IRAM_ATTR gpio_isr_handler(void* arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xGuiSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
```
Next, we create our task. This task is responsible for handling the actual work that should happen after the button press event. It first configures the LED GPIO as an output, then enters an infinite loop where it waits for a signal from the semaphore. When the ISR gives the semaphore, this task wakes up, toggles the LED state.
```c
void led_toggle_task(void *pvParameters) {
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    int state = 0;

    while (1) {
        if (xSemaphoreTake(xGuiSemaphore, portMAX_DELAY) == pdPASS) {
            state = !state;
            gpio_set_level(LED_GPIO, state);
        }
    }
}
```
Finally, in the `app_main()` function, we first create a binary semaphore using `xSemaphoreCreateBinary()`. This semaphore will be used for synchronization between the interrupt service routine (ISR) and a task. After creation, we check whether the semaphore was successfully allocated by verifying that the returned handle is not `NULL`. If it is `NULL`, it means the system failed to allocate the required resources.

Next, we configure the GPIO using a `gpio_config_t` structure. This structure defines all the settings for the selected pin:

- `intr_type = GPIO_INTR_NEGEDGE` configures the interrupt to trigger on a falling edge (i.e., when the signal changes from high to low), which is typically used for button press detection when the pin is pulled high and pressed to ground.
- `mode = GPIO_MODE_INPUT` sets the pin as an input, allowing it to read external signals such as a button state.
- `pin_bit_mask = (1ULL << BUTTON_GPIO)` selects which GPIO pin is being configured by setting the corresponding bit in a 64-bit mask.
- `pull_up_en = 1` enables the internal pull-up resistor, ensuring the pin stays at a stable HIGH level when the button is not pressed.

After filling the configuration structure, we apply it using `gpio_config(&io_conf)`, which writes these settings to the hardware registers and activates the GPIO configuration.

Then, we initialize the GPIO interrupt system by calling `gpio_install_isr_service(0)`. This function sets up the interrupt service framework so that GPIO interrupt handlers can be registered. The argument `0` indicates default configuration flags are used.

Finally, we attach an interrupt handler to the selected GPIO pin using `gpio_isr_handler_add(BUTTON_GPIO, gpio_isr_handler, NULL)`. This links the specified GPIO pin to the ISR function `gpio_isr_handler`, which will be executed automatically whenever the configured interrupt event occurs. The last parameter (`NULL`) is a user-defined argument passed to the ISR, which is not used in this case.
```c
void app_main(void) {
    
    xGuiSemaphore = xSemaphoreCreateBinary();

    if (xGuiSemaphore != NULL) {
        
        gpio_config_t io_conf = {
            .intr_type = GPIO_INTR_NEGEDGE, // Trigger on press (High to Low)
            .mode = GPIO_MODE_INPUT,
            .pin_bit_mask = (1ULL << BUTTON_GPIO),
            .pull_up_en = 1,
        };
        gpio_config(&io_conf);

        
        gpio_install_isr_service(0);
        gpio_isr_handler_add(BUTTON_GPIO, gpio_isr_handler, NULL);
        xTaskCreatePinnedToCore(led_toggle_task, "LED_Task", 2048, NULL, 10, NULL, 1);
    }
}
```

## Memory
When we work with the ESP32-S3, understanding how memory operates is fundamental. Memory is where our program’s code, variables, and data live while the chip is running. To picture this, we can divide our memory into two main workspaces: Flash memory, which acts as our permanent filing cabinet, and RAM (Random Access Memory), which serves as our fast, temporary workbench.

The ESP32-S3 manages these workspaces using flexible memory mapping and two distinct pathways: an instruction bus used exclusively for executing code, and a data bus used for reading and writing information byte by byte.
### Flash Memory
Flash memory is our long-term storage. It securely holds our program code, saved settings, and permanent data so that everything remains safe even when the board loses power. Because internal RAM is limited, we rely heavily on Flash to hold the bulk of our application.
- **IROM (Code Executed from Flash):** This is where most of our executable binary code resides. Since we cannot fit everything into internal RAM, the ESP32-S3 uses a special caching system to map this code directly to the instruction space. This allows us to execute our application directly from Flash almost as quickly as if it were in internal memory.
- **DROM (Data Stored in Flash):** This is our read-only data section. We use DROM to store constant variables and fixed data that our program needs to read but will never change or execute.
### RAM
RAM is exceptionally fast and is used for active calculations and temporary variables. However, standard RAM loses all its information when the power is turned off. The ESP32-S3 divides our RAM workbench into several specialized areas depending on what we need to achieve:
- **DRAM (Data RAM):** This is our primary workspace for non-constant static data, zero-initialized data, and our runtime heap. DRAM is strictly connected to the data bus, meaning we cannot execute code from here. If we need certain data to survive a software restart (a warm boot), we can set aside a "noinit" section within DRAM to prevent it from being erased upon startup. Furthermore, when our hardware peripherals require direct memory access (DMA) to send or receive data rapidly, we must carefully place our memory buffers here in DRAM to ensure they are properly aligned and formatted.
- **IRAM (Instruction RAM):** Unlike DRAM, IRAM is executable. We reserve this highly valuable space for our most critical tasks, such as interrupt handlers or timing-sensitive code. By placing these operations in IRAM, we avoid the slight delays of fetching instructions from Flash. It is a balancing act, however; any space we consume for IRAM reduces the amount of DRAM available for our static data and heap.
- **RTC Memory (Real-Time Clock):** This is a special, low-power subset of RAM that remains active even when we put the rest of the chip into deep sleep.
    - **RTC Fast Memory:** We use this area for code and data that must execute immediately when the chip wakes up from deep sleep. If we do not need it for wake-up routines, we can add the leftover space to our general data heap, though it operates slightly slower than standard DRAM.
    - **RTC Slow Memory:** We utilize this deeper storage section for global and static variables that must retain their values throughout a deep sleep cycle, or for data that needs to be accessed by the Ultra Low Power (ULP) coprocessor while the main processor is resting.

### Working with Ram
Now that we understand the general structure of memory in the ESP32-S3, let’s explore how RAM works and how we can manipulate it. The ESP32-S3 divides RAM into two main regions: the heap and the stack.

#### Stack
Throughout our lectures, we have been working with the stack without even noticing it. Every time we create a variable inside a function, it is typically stored in the stack, which is used for local and automatic memory allocation. Whenever a function is called, the microcontroller automatically reserves a portion of stack memory for that function’s local variables, function arguments, and return addresses. Once the function finishes executing, that memory is automatically released.

The stack operates using a **Last-In, First-Out (LIFO)** mechanism, meaning the most recently added data is the first to be removed. Because of its simple structure, stack memory is extremely fast and efficient. However, the stack size is limited compared to the heap. Declaring very large local variables or deeply nested function calls can exhaust the available stack space, causing a stack overflow. On the ESP32-S3, a stack overflow usually results in a system panic, crash, or automatic reboot.

#### Working with the Stack
Let’s create a simple example to understand how the stack works. In this example, one function calls another function, allowing us to observe how the stack is managed during function execution.
```c
#include <stdio.h>  
  
void secondFunction() {  
	int b = 20;  
	printf("b = %d\n", b);  
}  
  
void firstFunction() {  
	int a = 10;  
	printf("a = %d\n", a); 
	secondFunction();  
}  
  
void app_main(void) {  
	firstFunction();  
}
```
When `app_main()` calls `firstFunction()`, the microcontroller creates a new stack frame containing the local variable `a`, the function parameters, and the return address. While `firstFunction()` is still running, it calls `secondFunction()`, causing another stack frame to be placed on top of the previous one. This new frame stores the local variable `b` and the information needed for `secondFunction()` to execute.

<img src="./attachments/stack_memory.png" />

Once `secondFunction()` finishes, its stack frame is automatically removed, and the program returns to `firstFunction()`. After `firstFunction()` finishes, its stack frame is also released, and execution returns to `app_main()`.

<img src="./attachments/stack_free.png"/>

This process demonstrates why local variables exist only within their function, and why we cannot access a variable after its function has finished executing. After a function returns, its stack memory is released and may later be reused by other functions

#### Heap
Unlike the stack, which automatically allocates and releases memory during function calls, the heap is used for dynamic memory allocation. Memory in the heap is managed manually by the programmer, meaning we explicitly request memory when needed and release it when it is no longer required.    

Unlike the LIFO structure of the stack, the heap is a large pool of memory where blocks can be allocated and freed in any order. However, this flexibility comes with a cost. Allocating memory on the heap is slower than on the stack.

The heap is especially useful when the amount of memory required is not known at compile time. Instead of creating fixed-size local variables, we can dynamically allocate memory during program execution. This is very common in embedded systems when handling buffers, communication packets, sensor data, image processing, or dynamically sized arrays.

On the ESP32-S3, heap memory is managed by the ESP-IDF memory allocator. Functions such as `malloc()`, `calloc()`, `realloc()`, and `free()` are commonly used for dynamic memory management. ESP-IDF also provides advanced heap capabilities, allowing developers to allocate memory from different memory regions such as internal RAM, DMA-capable memory, or external PSRAM.

Unlike the stack, heap memory does not automatically disappear when a function finishes. The allocated memory remains reserved until it is explicitly released using `free()`. Failing to release unused heap memory causes a memory leak, which gradually reduces the available memory and can eventually crash the system.


#### Working with the Heap
Let’s create a simple example to understand how heap memory works on the ESP32-S3 using ESP-IDF.
```c
#include <stdio.h>
#include <stdlib.h>

void app_main(void) {
    // Allocate memory for 5 integers
    int *numbers = (int *)malloc(5 * sizeof(int));
    // Check if allocation was successful
	if (numbers == NULL){
		printf("Memory allocation failed!\n");
		return;
	}

   // Store values inside the allocated memory
    for (int i = 0; i < 5; i++){
        numbers[i] = (i + 1) * 10;
    }

    // Print the stored values
    for (int i = 0; i < 5; i++){
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }

    // Release the allocated memory
    free(numbers);

    printf("Heap memory released\n");
}
```
In this example, the `malloc()` function dynamically reserves memory from the heap large enough to store five integers. The returned pointer, `numbers`, contains the address of the allocated memory block.
```c
int *numbers = (int *)malloc(5 * sizeof(int));
```
Unlike stack variables, this memory remains valid even if it was allocated inside another function, as long as it has not been released using `free()`.

After allocating the memory, we store values inside the array normally using pointer indexing.
```c
numbers[i] = (i + 1) * 10;
```
Once the memory is no longer needed, we release it using `free()`.
```
free(numbers);
```
After calling `free()`, the heap manager marks that memory region as available again so it can later be reused by other parts of the program.

#### Monitoring Heap Memory in ESP-IDF
ESP-IDF provides built-in functions to monitor heap usage during runtime. This is extremely useful when debugging memory leaks or checking how much free memory is available.
```c
#include <stdio.h>
#include "esp_heap_caps.h"

void app_main(void){

   printf("Free heap before allocation: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

   int *data = malloc(100 * sizeof(int));
   printf("Free heap after allocation: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
   
   free(data);
   printf("Free heap after free(): %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

}
```
The function `heap_caps_get_free_size()` allows us to check the available heap memory in real time. This is particularly important on embedded systems where memory resources are limited.

#### Controlling Heap Allocation
While standard C functions like `malloc()` are easy to use, they have a major limitation in advanced embedded systems: they do not let us specify where in the microcontroller's hardware the memory should be allocated. The ESP32-S3 has a complex memory map that includes fast internal SRAM (divided into IRAM and DRAM) and often slower, but much larger, external PSRAM (SPI RAM).

To solve this limitation, ESP-IDF provides a powerful function called `heap_caps_malloc()`. This function allows us to explicitly dictate which memory region our data should be placed in by applying specific capability flags.
```c
#include "esp_heap_caps.h"

void *heap_caps_malloc(size_t size, uint32_t caps);
```
- **`size`**: The number of bytes you want to allocate.
- **`caps`**: A bitmask of memory capability flags that dictate where and how the memory is allocated.

Let's explore the most common use cases for controlling heap allocation on the ESP32-S3.

**1. Allocating from Internal RAM Only** Sometimes we have performance-critical data that must be accessed as fast as possible. By default, if external PSRAM is enabled, `malloc()` might place our data there. We can force the allocator to only use the fast internal DRAM:
```c
#include <stdio.h>
#include "esp_heap_caps.h"

void app_main(void) {
    // Force allocation strictly in fast internal DRAM
    int *data = heap_caps_malloc(100 * sizeof(int), MALLOC_CAP_INTERNAL);

    if (data == NULL) {
        printf("Internal RAM allocation failed!\n");
        return;
    }

    // ... use the data ...

    free(data);
}
```
**2. Allocating from External PSRAM** The ESP32-S3's internal memory is precious and usually reserved for FreeRTOS tasks and fast operations. If we need a massive chunk of memory, we should push it to the external PSRAM. This is incredibly useful for:

- Large data buffers
- Image processing or camera framebuffers
- Audio data processing
- Large network packets

First thing we need to do is configuring and enabling the PSRAM, we do that by running the following command  

`idf.py menuconfig`   

After that we enable PSRAM as following

```
Component config
    ESP PSRAM
        [*] Support for external, SPI-connected RAM
```
Next we access to `Mode of SPI RAM` and we set it to `Octal Mode PSRAM`, finally we go to`SPI RAM config`, we go then to `SPI RAM access method` and we select
```
Make RAM allocatable using heap_caps_malloc..
```
Now we ready to use the PSRAM memory
```c
// Allocate a massive buffer in external PSRAM
int *buffer = heap_caps_malloc(10000 * sizeof(int), MALLOC_CAP_SPIRAM);

if (buffer == NULL) {
    printf("PSRAM allocation failed!\n");
} else {
    printf("Successfully allocated large buffer in PSRAM.\n");
    free(buffer);
}
```
**3. DMA-Safe Allocation** When working with hardware peripherals like SPI, I2S (audio), or LCD interfaces, the hardware often uses Direct Memory Access (DMA). DMA allows peripherals to read and write memory independently of the CPU, which drastically speeds up data transfers. However, DMA hardware can only access specific physical memory regions.

If we pass standard memory to a DMA controller, it might crash. We must request DMA-capable memory explicitly:
```c
// Allocate memory that is physically accessible by DMA hardware
uint8_t *dma_buffer = heap_caps_malloc(2048, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
```
Notice that we used the bitwise OR operator (`|`) to combine flags. This ensures the memory is both DMA-accessible and resides in internal RAM.   

**4. Combining Multiple Constraints** The bitwise OR (`|`) allows us to stack as many constraints as our application requires. For example, if we need memory that is guaranteed to be byte-accessible (8-bit) and strictly internal:
```c
// Must be accessible as normal 8-bit memory AND come from internal RAM
void *p = heap_caps_malloc(512, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
```

#### Heap Fragmentation
One important issue with dynamic memory allocation is heap fragmentation. Fragmentation occurs when many allocations and deallocations of different sizes leave small unused gaps in memory. Even if the total free memory appears large enough, there may not be a sufficiently large continuous block available for a new allocation.

On long-running ESP32-S3 applications, excessive fragmentation can eventually cause allocation failures. To reduce fragmentation:

- Avoid repeatedly allocating and freeing memory in fast loops.
- Reuse buffers whenever possible.
- Prefer fixed-size memory pools for predictable systems.
- Free unused memory as soon as possible.
- Monitor heap usage during runtime.

Efficient heap management is very important in embedded systems because memory resources are limited compared to desktop computers. Poor memory management can lead to crashes, instability, unexpected reboots, or reduced system performance.
### Task and Memory
Now we must connect our understanding of memory to the FreeRTOS concepts we learned earlier. In a Bare-Metal system, there is only one main stack for the whole program. But in an RTOS, every single task gets its own dedicated Stack.

When we call `xTaskCreatePinnedToCore()`, where does the memory for the new task's stack come from? It is carved out of the Heap. If we tell a task it needs 4096 bytes of stack space, the RTOS uses `malloc` behind the scenes to reserve 4096 bytes of heap memory specifically for that task's local variables.

Because stack overflows are the most common bug in ESP-IDF development, FreeRTOS provides a function called `uxTaskGetStackHighWaterMark()`. This function tells us the minimum amount of remaining stack space that was ever available to the task since it started. If this number gets close to zero, we need to increase the task's stack size.
```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void memory_monitor_task(void *pvParameters) {
    int local_array[100]; // Consumes 400 bytes on this task's stack
    
    while (1) {
        // Pass NULL to check the stack of the currently running task
        UBaseType_t high_water_mark = uxTaskGetStackHighWaterMark(NULL);
        
        // This prints the number of *bytes* (or words depending on architecture) remaining
        printf("Task Stack High Water Mark: %u\n", high_water_mark);
        
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void) {
    // We allocate 2048 bytes from the Heap to serve as this task's Stack
    xTaskCreatePinnedToCore(memory_monitor_task, "MemTask", 2048, NULL, 1, NULL, 1);
}
```


### Working with Flash and Persistent Data Storage
Now that we understand how RAM works and how it temporarily holds our data while the program is running, let’s explore how the ESP32-S3 handles data that needs to survive a power cycle or reboot. Unlike RAM, which is volatile and wiped clean when the device loses power, Flash memory is non-volatile. This means it retains its data indefinitely without power.

The ESP32-S3 uses external SPI Flash memory and sometimes internal embedded Flash to store the compiled application code, bootloader, and persistent user data. However, we cannot simply throw data anywhere into the Flash memory; it must be highly organized.

#### The Partition Table
Because the Flash memory holds several critical pieces of data, it is divided into distinct sections or "partitions." Think of the partition table as a map or an index that tells the ESP32-S3 exactly where everything lives.

When we compile an ESP-IDF project, a `.csv` file defines this layout. A typical partition table divides the Flash into:
- **Bootloader:** The code that runs immediately on startup to load the main application.
- **Factory App:** The main executable compiled code of our program.
- **OTA Partitions:** Reserved spaces for Over-The-Air updates, allowing the device to download a new app while the old one is still running.
- **NVS (Non-Volatile Storage):** A small partition used for saving configuration settings.
- **Data Partitions:** Larger spaces formatted for file systems like SPIFFS or LittleFS.

By structuring the Flash memory this way, the microcontroller ensures that writing a text file to our data partition doesn't accidentally overwrite our application code.    
Here is an example of what a `partitions.csv` file looks like:
```
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     ,        0x6000,
phy_init, data, phy,     ,        0x1000,
factory,  app,  factory, ,        1M,
storage,  data, spiffs,  ,        1M,
```

#### NVS (Non-Volatile Storage)
The Non-Volatile Storage (NVS) library is ESP-IDF's tool for saving small pieces of data. It operates like a dictionary, using **Key-Value pairs**. We save a piece of data (the value) and give it a unique string name (the key). Later, we can ask NVS to give us the value associated with that exact key.

NVS is heavily optimized for flash memory. Because Flash memory can only be erased a certain number of times before it degrades, NVS uses a technique called wear leveling to distribute read/write operations evenly across the flash sectors, extending the life of our hardware. NVS is perfect for saving Wi-Fi credentials, calibration offsets, or state variables (like a restart counter).

#### Working with NVS
Let’s look at a simple example of using NVS to build a small project that counts user button presses and keeps the count saved even after the device is rebooted.

In this project, we will create a simple circuit with two buttons:

- **Count Up button** connected to **GPIO 4**
- **Reset button** connected to **GPIO 5**

Each time the Count Up button is pressed, the counter value increases and is stored in NVS. The Reset button clears the stored count and sets the counter back to zero.


<img src="./attachments/circuit_nvs.png" />

Now let's create the program for our application


```c
#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void set_counter(int32_t val){
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        nvs_set_i32(my_handle, "counter", val);
        nvs_commit(my_handle);
        nvs_close(my_handle);
    }
}


void app_main(void) {
    gpio_reset_pin(5);
    gpio_reset_pin(4);

    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_set_direction(5, GPIO_MODE_INPUT);

    gpio_set_pull_mode(5,GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(4,GPIO_PULLDOWN_ONLY);
    
    
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    nvs_handle_t my_handle;
    int32_t counter = 0;

    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        nvs_get_i32(my_handle, "counter", &counter);
        printf("counter  = %ld \n", counter);
        nvs_close(my_handle);
    }
    
    while (1){
	    if(gpio_get_level(4)){
			counter++;
	        set_counter(counter);
	        printf("counter  is now %ld.\n", counter);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        
	    }
		if(gpio_get_level(5)){
		    counter = 0;
	        set_counter(counter);
			printf("counter  reset to 0 \n count =  %ld.\n", counter);
	        vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
}
```
To work with NVS (Non-Volatile Storage), we first need to include the required libraries. In this example, we added two new header files: `nvs_flash.h` and `nvs.h`. These libraries provide the functions needed to initialize, read, write, and manage data stored in NVS memory.

Inside `app_main()`, we begin by initializing the NVS flash using `nvs_flash_init()`. This prepares the flash memory so it can be used for data storage. We also check for possible initialization errors, such as running out of free pages or detecting an old NVS version. If either problem occurs, we erase the NVS partition and initialize it again.

Next, we create an NVS handle (`my_handle`) and a variable named `counter` with a default value of `0`. The handle acts like a connection to the NVS storage. We then open an NVS namespace called `"storage"` using the `NVS_READWRITE` flag, which allows us to both read and write data. After opening the namespace, we retrieve the saved counter value using `nvs_get_i32()` and store it inside the `counter` variable. The value is printed to the terminal, and the NVS handle is closed afterward.

The program then enters an infinite loop that continuously checks the state of the two buttons. When the button connected to GPIO 4 is pressed, the counter value increases by one. The updated value is stored in NVS using the `set_counter()` function, and the new counter value is printed to the terminal. When the button connected to GPIO 5 is pressed, the counter is reset to `0`, saved again to NVS, and the updated value is displayed.

The `set_counter()` function is responsible for saving the counter value into NVS. It accepts one argument, `val`, which represents the value we want to store. Inside the function, we open the `"storage"` namespace, use `nvs_set_i32()` to save the integer value under the key `"counter"`, and then call `nvs_commit()` to permanently write the changes to flash memory. Finally, we close the NVS handle using `nvs_close()` to free the resources.

NVS also supports strings, we can store strings by using `nvs_set_str()`
```c
char ssid[] = "MyWiFi";
nvs_set_str(my_handle, "wifi_ssid", ssid);
nvs_commit(my_handle);
```
To read string we use `nvs_get_str` since we not sure about the size of stored string we first use the function to retrive the size of string, then we create bugger with same size and read the string
```c
size_t required_size;
nvs_get_str(my_handle, "wifi_ssid", NULL, &required_size);
char *buffer = malloc(required_size);
nvs_get_str(my_handle, "wifi_ssid", buffer, &required_size);
printf("SSID: %s\n", buffer);
free(buffer);
```
#### SPIFFS & LittleFS
While NVS is excellent for tiny variables, it is terrible for large datasets. If we are building a web server and need to store HTML, CSS, and image files, or if we are logging thousands of temperature readings to a text file, we need a true File System.

ESP-IDF primarily supports two file systems for internal Flash:
- **SPIFFS (SPI Flash File System):** An older, widely used file system. It provides wear leveling but does not support true directories. If er create a file at `/data/logs/temp.txt`, SPIFFS simply treats the entire string `"/data/logs/temp.txt"` as the file name.
- **LittleFS:** A newer, highly reliable file system designed specifically for microcontrollers. Unlike SPIFFS, LittleFS supports actual directory structures, is significantly faster, and is highly resilient to power loss during write operations. Because of its superior performance and safety, LittleFS is generally recommended over SPIFFS for modern ESP32-S3 projects.

#### Working with LittleFS
By default, ESP-IDF does not reserve any flash memory for a custom file system. To use LittleFS, we need to create a custom partition table that allocates a dedicated region of flash memory for storage.

Create a file named `partitions.csv` in the root of the project directory next to `CMakeLists.txt` with the following content:
```csv
# Name, Type, SubType, Offset, Size, Flags
nvs, data, nvs, , 0x6000,
phy_init, data, phy, , 0x1000,
factory, app, factory, , 1M,
storage, data, spiffs, , 1M,
```
In this table, the `storage` partition defines a 1 MB flash region that will be used by LittleFS. Even though the subtype is listed as `spiffs`, the partition can still be mounted and used with LittleFS.

Next, we need to tell ESP-IDF to use our custom partition table and include the `esp_littlefs` component in the project. We can do this using the ESP-IDF Component Manager by creating a file named `idf_component.yml` inside the `main/` directory.

The ESP-IDF Component Manager will automatically download and integrate the LittleFS component during the build process.
```
dependencies:
  joltwallet/littlefs: "~=1.21.0"
```
After that, enable the custom partition table configuration by running the following command in the terminal:
```
idf.py menuconfig
```
Inside the configuration menu, navigate to:
```
Partition Table -> Partition Table
```
Then change the option from:
```
Single factory app, no OTA
```
To:
```
Custom partition table CSV
```
Since our application uses more than **2 MB** of flash memory, we need to update the serial flash configuration.

First, open:
```
Serial flasher config
```
Then navigate to:
```
Flash size
```
Select a flash size that is **larger than 2 MB** and matches the flash capacity of your ESP32 board.

If you are unsure how much flash your board has, check the board specifications or the module's documentation. Once you know the available flash size, make sure the **Partition Table** is configured to fit within that capacity. The total size of all partitions must not exceed the flash memory available on the board.

Finally, save the configuration and exit the menu.

Now that we have finished the configuration, let's build a small project to see LittleFS in action. In this example, we will create a file, flash it into the ESP32's LittleFS partition, and then read it back. This is the same approach commonly used by web server projects to store HTML, CSS, JavaScript, images, and other static files.

First, create a folder named `littlefs` in your project directory. Inside this folder, create a file named `hello.txt` and add the following content:
```
Hello world
```
Next, create the following program:
```c
#include <stdio.h>
#include <string.h>
#include "esp_littlefs.h"

void app_main(void) {
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",      
        .partition_label = "storage",   
        .format_if_mount_failed = true,
        .dont_mount = false,
    };
    esp_vfs_littlefs_register(&conf);

    
   FILE *f = fopen("/littlefs/hello.txt", "r");
    if (f == NULL) {
        printf("Failed to open file for reading\n");
        return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    printf("Read from file: '%s'\n", line);
    esp_vfs_littlefs_unregister(conf.partition_label);
}

```
The first step is creating an `esp_vfs_littlefs_conf_t` configuration structure. This structure tells ESP-IDF how the LittleFS file system should be mounted. The `base_path` field specifies the virtual directory where the file system will appear in our application, while `partition_label` must match the partition name defined in `partitions.csv`.

The `format_if_mount_failed` option is particularly useful during development. If the partition is empty or becomes corrupted, ESP-IDF will automatically format it instead of failing to mount. In a production application, you would typically set this option to `false` to avoid accidentally erasing stored data.

Next, we mount the file system by calling `esp_vfs_littlefs_register()`. Once the mount is successful, the partition becomes accessible through ESP-IDF's Virtual File System (VFS), allowing us to use the standard C file functions such as `fopen()`, `fgets()`, `fprintf()`, and `fclose()`.

To read the file, we call `fopen()` with the `"r"` mode to open `hello.txt` for reading. If the file cannot be opened, the program prints an error message and exits. Otherwise, `fgets()` reads the contents of the file into a character buffer, and we remove the trailing newline before printing the result to the serial monitor.

Finally, we unmount the file system using `esp_vfs_littlefs_unregister()`. Although many embedded applications keep the file system mounted for the entire lifetime of the program, unmounting it is good practice when the application is shutting down or before restarting the device.


Before compiling the project, we also need to tell the build system to create a LittleFS image from the `littlefs` folder and include it when flashing the ESP32.

To do this, open the project's top-level `CMakeLists.txt` file and add the following line:
```
littlefs_create_partition_image(storage littlefs FLASH_IN_PROJECT)
```
The first argument, `storage`, is the partition label and **must** match the partition name defined in `partitions.csv`. The second argument, `littlefs`, specifies the folder containing the files that will be packaged into the LittleFS image. Finally, the `FLASH_IN_PROJECT` option instructs ESP-IDF to automatically flash the generated LittleFS image whenever you run `idf.py flash`, so there is no need to flash the file system separately.

After adding this line, build and flash the project as usual. During the build process, ESP-IDF will package everything inside the `littlefs` folder into a LittleFS image, flash it to the `storage` partition, and your application will be able to access those files at runtime.

#### Raw Flash Partition Access
While filesystems like LittleFS and SPIFFS are excellent for managing files and directories, they introduce overhead. If we need to log massive arrays of raw sensor metrics, stream binary data at high speeds, or manage custom cryptographic blobs, bypassing the Virtual File System (VFS) entirely gives us absolute control and maximum performance.

Accessing raw flash memory allows us to read and write directly to specific physical sectors of the SPI flash chip via the ESP-IDF Partition API.

#### The Rules of Raw Flash
Working with raw flash is highly efficient, but it requires adherence to the physical constraints of NOR flash memory:
- **Erase Before Write:** Flash memory cells can only be flipped from a logical `1` to a `0` during a write operation. To turn a `0` back into a `1`, the entire sector must be erased. We cannot overwrite an existing byte without erasing its sector first.
- **Sector Alignment:** Erase operations cannot target individual bytes; they operate strictly on **4 KB sectors** (4096 bytes). Any erase offset and size must be perfectly aligned to a 4 KB boundary.
- **Wear Management:** Unlike LittleFS, raw partition access does not automatically include wear leveling. Writing continuously to the exact same sector will prematurely degrade that area of the flash chip.

#### Configuring a Raw Partition
To prevent the application from accidentally overwriting system code or NVS data, we must allocate a dedicated sandbox partition in our custom configuration.

In `partitions.csv` file we add a custom data entry:
```
# Name, Type, SubType, Offset, Size, Flags
nvs, data, nvs, , 0x6000,
phy_init, data, phy, , 0x1000,
factory, app, factory, , 1M,
raw_data, data, 0x99, , 256K,
```
In this setup, we define a partition labeled `raw_data`. We use the generic `data` type, and assign a custom hexadecimal subtype (`0x99`) to clearly define it as a non-filesystem, raw storage space. Its size is restricted to 256 KB.
#### Reading and Writing Raw Flash
To interact with this partition, include the `esp_partition.h` header. The workflow involves locating the partition by its label, erasing the target sector, writing binary data, and reading it back.

The following example demonstrates a safe raw flash transaction:
```c
#include <stdio.h>  
#include <string.h>  
  
#include "esp_partition.h"  
  
void app_main(void) {  
  
	// 1. Find the custom partition  
	const esp_partition_t *partition =  esp_partition_find_first(  
		ESP_PARTITION_TYPE_DATA,  
		0x40,  
		"storage"  
	);  
  
	if (partition == NULL) {  
		printf("Partition not found\n");  
		return;  
	}  
  
	printf("Partition found: %s\n", partition->label);  
  
	// 2. Data to write  
	const char *message = "Hello from raw flash memory!";  
  
	// 3. Erase flash sector before writing  
	esp_err_t ret = esp_partition_erase_range(  
		partition,  
		0,  
		4096  
	);  
  
	if (ret != ESP_OK) {  
		printf("Failed to erase partition\n");  
		return;  
	}  
  
	// 4. Write data to flash  
	ret = esp_partition_write(  
		partition,  
		0,  
		message,  
		strlen(message) + 1  
	);  
  
	if (ret != ESP_OK) {  
		printf("Failed to write data\n");  
		return;  
	}  
  
	printf("Data written successfully.\n");  
  
	// 5. Read data back  
	char buffer[64];  
  
	ret = esp_partition_read(  
		partition,  
		0,  
		buffer,  
		sizeof(buffer)  
	);  
  
	if (ret != ESP_OK) {  
		printf("Failed to read data\n");  
		return;  
	}  
  
	printf("Read from flash: '%s'\n", buffer);  
}
```
The first step is locating the partition using `esp_partition_find_first()`. We search for a partition with type `ESP_PARTITION_TYPE_DATA`, subtype `0x40`, and label `"storage"`.

If the partition exists, ESP-IDF returns a pointer to an `esp_partition_t` structure containing information about the partition, including its address, size, and label.

Before writing to flash memory, we must erase the target region using `esp_partition_erase_range()`. Flash memory cannot overwrite existing data directly. Instead, flash sectors must first be erased, typically in 4 KB blocks.

After erasing the sector, we write raw bytes into flash using `esp_partition_write()`. In this example, we store a simple text string, but the same method can be used for binary structures, arrays, or serialized objects.

To verify the operation, we read the data back using `esp_partition_read()`. The data is copied into a RAM buffer and printed to the serial monitor.

Unlike LittleFS or SPIFFS, raw flash access does not provide files, folders, or automatic memory management. Everything is handled manually, which makes this method extremely powerful but also more error-prone if offsets and erase operations are not carefully managed.


#### Flash Degradation 
Just as heap fragmentation is a danger to RAM, Flash wear is the main danger to non-volatile storage.

Flash memory physically degrades over time. Every time we erase and write a sector, the silicon takes a tiny bit of microscopic damage. A typical Flash chip is rated for about 100,000 erase/write cycles per sector.

If we were to log a temperature reading to the exact same raw Flash address every second, we would destroy that specific sector of the Flash chip in just over a day. To prevent this, NVS, SPIFFS, and LittleFS all utilize **wear leveling**. Wear leveling algorithms secretly map our data to different physical locations behind the scenes, ensuring that writes are spread out evenly across the entire chip, significantly extending the life of our hardware.
