#include <stdio.h>
#include <string.h>

#include "esp_littlefs.h"

void app_main(void) {

    // 1. Configure the LittleFS mount
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",       // The root path we will use in code
        .partition_label = "storage",   // Must match the name in partitions.csv
        .format_if_mount_failed = true, // Format the partition if it's empty/corrupted
        .dont_mount = false,
    };
    // 2. Initialize and mount the file system
    esp_vfs_littlefs_register(&conf);

    
   FILE *f = fopen("/littlefs/hello.txt", "r");
    if (f == NULL) {
        printf("Failed to open file for reading\n");
        return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // Remove newline character if present
    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    printf("Read from file: '%s'\n", line);
    // 5. Unmount (optional, usually done before a system reboot)
    esp_vfs_littlefs_unregister(conf.partition_label);
}
