# Heap Memory Allocation
This example demonstrates how to dynamically allocate and release memory on the ESP32-S3 using the heap. Unlike stack memory, which is automatically managed during function calls, heap memory is allocated manually at runtime, allowing applications to request memory only when it is needed. This makes the heap ideal for handling data whose size is not known until the program is running.

The project allocates enough heap memory to store five integers using the `malloc()` function. Before using the allocated memory, the program verifies that the allocation was successful by checking whether the returned pointer is `NULL`. If the allocation succeeds, values are stored inside the allocated array using normal array indexing, and the contents are printed to the serial monitor.

Once the allocated memory is no longer required, the program releases it by calling `free()`. Releasing unused memory allows the ESP-IDF heap manager to reuse that memory for future allocations and helps prevent memory leaks that could eventually exhaust the available RAM.
