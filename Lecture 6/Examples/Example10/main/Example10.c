#include <stdio.h>
#include "esp_heap_caps.h"
#include <stdlib.h>

void app_main(void){
int *buffer = heap_caps_malloc(10000 * sizeof(int), MALLOC_CAP_SPIRAM);

if (buffer == NULL) {
    printf("PSRAM allocation failed!\n");
} else {
    printf("Successfully allocated large buffer in PSRAM.\n");
    free(buffer);
}
}
