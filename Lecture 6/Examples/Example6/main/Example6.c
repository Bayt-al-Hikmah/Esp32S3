#include <stdio.h>

void secondFunction() {
	int b = 20;
	printf("b = %d\n", b);
}

void firstFunction() {
	int a = 10;
	printf("a = %d\n", a);
	secondFunction();
}

void app_main(void) {
	firstFunction();
}
