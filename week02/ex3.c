#include<stdio.h>
#include <math.h>

void convert(long long int x, int s, int t){
    
    if (s > 10 || s < 2 || t > 10 || t < 2){
        printf("cannot convert!");
        return;
    }
    
    int x_copy = x;
    int size = 0;
    while (x_copy > 0){
        int remaind = x_copy % 10;
        if (remaind >= s){
            printf("cannot convert!");
            return;
        }
        size++;
        x_copy = x_copy / 10;
    }
    
    char array[size];
    sprintf(array, "%lld", x);
    
    int x_decimal = 0;
    for (int i =0; i < size; i++){
        int current_digit = array[size - 1 - i] - '0';
        x_decimal += current_digit * (int)pow(s, i);
    }
    
    size = 0;
    
    x_copy = x_decimal;
    while(x_copy > 0){
        int remaind = x_copy % t;
        size++;
        x_copy = x_copy / t;
    }
    
    char result[size];
    for (int i = 0; i < size; i++){
        int remaind = x_decimal % t;
        x_decimal = x_decimal / t;
        result[i] = (int)remaind ;
    }
    
    for (int i = size -1; i >= 0; i--){
        printf("%d", result[i]);
    }
    

}

int main()
{
    long long int number;
    int source;
    int target;
    
    scanf("%lld %d %d", &number, &source, &target);
    convert(number, source, target);
    
        
    return 0;
}
