#include<stdio.h>

int main()
{
    
    char arr[256];
    int size = 0;
    
    for (int i =0; i < 256; i ++){
        arr[i] = getchar();
        
        
        if (arr[i] == '.' || arr[i] == '\n'){
            break;
        }
        
        size++;
    }
    
    for (int i = size - 1; i >= 0; i--){
        printf("%c", arr[i]);
    }
        
    return 0;
}
