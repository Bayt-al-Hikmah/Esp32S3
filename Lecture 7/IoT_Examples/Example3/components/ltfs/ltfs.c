
#include "ltfs.h"

void init_littlefs(void){
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/web",       
        .partition_label = "storage",   
        .format_if_mount_failed = true, 
        .dont_mount = false,
    };

    esp_vfs_littlefs_register(&conf);
}