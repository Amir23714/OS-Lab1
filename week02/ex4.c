#include<stdio.h>
#include <stdbool.h>
#include <ctype.h> 

int count(char string[], int size ,char target){
    
    int cnt = 0;
    
    for (int i = 0 ; i < size; i ++){
        if (tolower(string[i]) == target){
            cnt++;
        }
    }
    return cnt;
}

void countAll(char string[], int size) {
    
    for (int i = 0; i < size; i++){
        
        if (i ==0) {
            int cnt = count(string, size, tolower(string[i]));
            printf("%c:%d", tolower(string[i]), cnt);
            continue;
        }
        
        int cnt = count(string, size, tolower(string[i]));
        printf(", %c:%d", tolower(string[i]), cnt);
            
        
    }
}

int main()
{
    char sample[256];
    int size = 0;
    
    for (int i =0; i < 256; i ++){
        sample[i] = getchar();
        
        
        if (sample[i] == '\n'){
            break;
        }
        
        size++;
    }
    
    countAll(sample, size);
        
    return 0;
}
