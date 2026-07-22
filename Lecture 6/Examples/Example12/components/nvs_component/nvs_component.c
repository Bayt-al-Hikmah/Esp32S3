#include "nvs_component.h"


void set_counter(int32_t val){
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        nvs_set_i32(my_handle, "counter", val);
        nvs_commit(my_handle);
        nvs_close(my_handle);
    }
}

void init_nvs(int32_t *counter){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        nvs_get_i32(my_handle, "counter", counter);
        printf("counter  = %ld \n", *counter);
        nvs_close(my_handle);
    }
}
