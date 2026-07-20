# Allocating Memory from Internal RAM

This example demonstrates how to allocate memory specifically from the ESP32-S3's internal RAM using the ESP-IDF heap capabilities allocator. Unlike `malloc()`, which allocates memory from any suitable heap region, `heap_caps_malloc()` allows the programmer to specify the type of memory required. In this example, the `MALLOC_CAP_INTERNAL` capability ensures that the allocated memory comes only from the fast internal DRAM.
