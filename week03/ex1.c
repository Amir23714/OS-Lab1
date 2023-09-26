#include <stdio.h>

int const_tri(int* p, int* n){
    int zero = *p++;
    int one = *p++;
    int two = *p++;
    
    int current;
    
    for (int i = 3; i <= *n; i++){
        current = zero + one + two;
        
        zero = one;
        one = two;
        two = current;
    }
    
    return current;
}

int main()
{
    const int x = 1;
    int * q = &x;
    
    int arr[3];
    
    int * p = &arr[0];
    
    *p++ = *q;
    *p++ = *q;
    *p = *q * 2;
    
    printf("%p %p %p", &arr[0], &arr[1], &arr[2]);

    return 0;
}
