#include <stdbool.h>

int extract_index(char* buffer, char* argument, int size){
    bool match = false;
    for (int i = 0; buffer[i] != '\0'; i++){
        if(buffer[i] == argument[0]){
            match = true;
            for (int j = 1; buffer[j] != '\0' && j < size; j++){
                if(buffer[i+j] != argument[j]){
                    match = false;
                    break ;
                }
            }
            if (match){
                return i + size;
            }
        }
    }
    return -1;
}