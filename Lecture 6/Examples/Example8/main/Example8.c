#include <stdio.h>
#include "esp_heap_caps.h"

void app_main(void){

   printf("Free heap before allocation: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

   int *data = malloc(100 * sizeof(int));
   printf("Free heap after allocation: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
   
   free(data);
   printf("Free heap after free(): %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

}
