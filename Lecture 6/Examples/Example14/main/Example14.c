#include <stdio.h>  
#include <string.h>  
  
#include "esp_partition.h"  
  
void app_main(void) {  
  
	// 1. Find the custom partition  
	const esp_partition_t *partition =  esp_partition_find_first(  
		ESP_PARTITION_TYPE_DATA,  
		0x99,  
		"raw_data"  
	);  
  
	if (partition == NULL) {  
		printf("Partition not found\n");  
		return;  
	}  
  
	printf("Partition found: %s\n", partition->label);  
  
	// 2. Data to write  
	const char *message = "Hello from raw flash memory!";  
  
	// 3. Erase flash sector before writing  
	esp_err_t ret = esp_partition_erase_range(  
		partition,  
		0,  
		4096  
	);  
  
	if (ret != ESP_OK) {  
		printf("Failed to erase partition\n");  
		return;  
	}  
  
	// 4. Write data to flash  
	ret = esp_partition_write(  
		partition,  
		0,  
		message,  
		strlen(message) + 1  
	);  
  
	if (ret != ESP_OK) {  
		printf("Failed to write data\n");  
		return;  
	}  
  
	printf("Data written successfully.\n");  
  
	// 5. Read data back  
	char buffer[64];  
  
	ret = esp_partition_read(  
		partition,  
		0,  
		buffer,  
		sizeof(buffer)  
	);  
  
	if (ret != ESP_OK) {  
		printf("Failed to read data\n");  
		return;  
	}  
  
	printf("Read from flash: '%s'\n", buffer);  
}
