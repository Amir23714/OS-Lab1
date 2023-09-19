#include<stdio.h>

int tribonacci(int n){
    if (n > 37 || n < 0){
        return -1;
    }
    
    int zero = 0;
    int one = 1;
    int two = 1;
    int current;
    
    for (int i = 3; i <= n; i++){
        current = two + one + zero;
        zero = one;
        one = two;
        two = current;
    }
    
    return current;
}

int main()
{
    printf("%d\n", tribonacci(4));
    printf("%d\n", tribonacci(36));
    return 0;
}
