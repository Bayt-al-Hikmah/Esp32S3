# Monitoring Heap Memory

This example demonstrates how to monitor the available heap memory on the ESP32-S3 during program execution using the ESP-IDF heap management API. Before allocating any memory, the program calls `heap_caps_get_free_size(MALLOC_CAP_8BIT)` to determine how many bytes of general-purpose heap memory are currently available. 

Next, the program dynamically allocates memory for one hundred integers using `malloc()`. After the allocation, it calls `heap_caps_get_free_size()` again to measure the remaining free heap memory. Since part of the heap has been reserved for the newly allocated array, the reported free memory decreases accordingly.

Once the allocated memory is no longer needed, the program releases it using `free()`. The heap size is then measured one final time, showing that the previously allocated memory has been returned to the heap and is once again available for future allocations. 


