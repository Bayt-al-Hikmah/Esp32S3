
#include <stdbool.h>

int extractNumber(char*buffer, int index){
    int number = 0;
    for (int k = index; buffer[k] != '\0'; k++){
        int digit = buffer[k] - '0';
        if(digit >= 0 && digit < 10){
            number = number * 10 + digit;
            
        }
        else{
            return number;
        }
    }
    return number;
}

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