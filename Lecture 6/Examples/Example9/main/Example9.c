#include <stdio.h>
#include "esp_heap_caps.h"

void app_main(void) {
    int *data = heap_caps_malloc(100 * sizeof(int), MALLOC_CAP_INTERNAL);

    if (data == NULL) {
        printf("Internal RAM allocation failed!\n");
        return;
    }


    free(data);
}
