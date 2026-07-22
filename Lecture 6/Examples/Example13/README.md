# Working with LittleFS

This example demonstrates how to use **LittleFS** to store and access files in the ESP32-S3's flash memory. Unlike NVS, which is designed for storing small configuration values, LittleFS provides a complete file system capable of organizing data into files and directories. This makes it well suited for applications that need to store web pages, images, configuration files, log files, or any other static resources that should remain available after the device is powered off.

Before LittleFS can be used, a dedicated flash partition must be reserved for the file system. This is done by creating a custom `partitions.csv` file in the project root and adding a **storage** partition. Although the partition subtype is listed as `spiffs`, it can still be mounted and used by the LittleFS driver.

Next, the LittleFS component must be added to the project by creating an `idf_component.yml` file inside the `main` directory with the appropriate dependency.
```
dependencies:
  joltwallet/littlefs: "~=1.21.0"
```
During the build process, the ESP-IDF Component Manager automatically downloads and integrates the `esp_littlefs` component into the project.

After adding the partition table and component dependency, configure the project by running:

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

Since the application now contains both the firmware and a dedicated LittleFS partition, ensure that the configured flash size matches your ESP32 module. Navigate to:

```
Serial flasher config
    Flash size
```

and select a flash size that is large enough to accommodate both the application and all defined partitions.

Before compiling the project, create a folder named `littlefs` in the project directory. Any files placed inside this folder will become part of the LittleFS image. To automatically package and flash these files, add the following line to the project's top-level `CMakeLists.txt`:

```
littlefs_create_partition_image(storage littlefs FLASH_IN_PROJECT)
```

This instructs ESP-IDF to generate a LittleFS image from the contents of the `littlefs` directory and automatically flash it to the **storage** partition whenever `idf.py flash` is executed.

Once the application starts, it mounts the **storage** partition using the LittleFS driver and registers it with ESP-IDF's **Virtual File System (VFS)**. After the file system is mounted, applications can use the standard C file functions such as `fopen()`, `fgets()`, `fprintf()`, and `fclose()` to access files exactly as they would on a desktop operating system. When the application has finished using the file system, it can safely unmount it to release the associated resources.
