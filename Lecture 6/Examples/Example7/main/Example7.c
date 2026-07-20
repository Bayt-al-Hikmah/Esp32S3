#include <stdio.h>
#include <stdlib.h>

void app_main(void) {
    int *numbers = (int *)malloc(5 * sizeof(int));

	if (numbers == NULL){
		printf("Memory allocation failed!\n");
		return;
	}


    for (int i = 0; i < 5; i++){
        numbers[i] = (i + 1) * 10;
    }

    for (int i = 0; i < 5; i++){
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }

    free(numbers);

    printf("Heap memory released\n");
}
