# Allocating Memory from External PSRAM

This example demonstrates how to allocate memory from the ESP32-S3's external PSRAM using the ESP-IDF heap capabilities allocator. External PSRAM provides a large memory pool that complements the limited internal RAM, making it ideal for applications that require large amounts of dynamic memory. Typical use cases include image processing, camera frame buffers, audio processing, large communication buffers, and other data-intensive applications where maximum performance is not the primary concern.

Before PSRAM can be used, it must be enabled in the ESP-IDF project configuration. Open the project configuration by running:

```
idf.py menuconfig
```

Then navigate to the following menu and enable external PSRAM:

```
Component config
    ESP PSRAM
        [*] Support for external, SPI-connected RAM
```

Next, set `Mode of SPI RAM` to `Octal Mode PSRAM`. Finally, navigate to `SPI RAM config`, open `SPI RAM access method`, and select:

```
Make RAM allocatable using heap_caps_malloc()
```

Once these settings are applied, We can use  `heap_caps_malloc()` with the `MALLOC_CAP_SPIRAM` to allocate memory from PSRAM.

