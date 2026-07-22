# Working with Raw Flash Partitions
This example demonstrates how to access the ESP32-S3's flash memory directly using the **ESP-IDF Partition API**. Unlike file systems such as LittleFS or SPIFFS, which organize data into files and directories, raw flash access bypasses the Virtual File System (VFS) entirely and allows applications to read and write directly to a dedicated flash partition. 

Before raw flash can be used, a dedicated partition must be reserved in the project's `partitions.csv` file. This partition acts as a protected storage area that is separate from the application firmware, NVS, and other system partitions. 

After creating the custom partition table, configure the project by running:

```
idf.py menuconfig
```

Navigate to:

```
Partition Table
    Partition Table
```

and change the partition table option from:

```
Single factory app, no OTA
```

to:

```
Custom partition table CSV
```

Once the partition table has been configured and flashed to the device, the application can locate the custom partition by its type, subtype, and label. After obtaining a reference to the partition, the ESP-IDF Partition API provides functions for erasing, writing, and reading data directly from flash memory without requiring a file system.

