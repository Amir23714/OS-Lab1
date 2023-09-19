#include<stdio.h>
#include <limits.h>
#include <float.h>

int main()
{
    int integer = INT_MAX;
    unsigned short int uinteger = USHRT_MAX;
    signed long int sinteger = LONG_MAX;
    float floatvar = FLT_MAX;
    double doublevar = DBL_MAX;
    
    printf("%d %ld\n", integer, sizeof(integer));
    printf("%u %ld\n", uinteger, sizeof(uinteger));
    printf("%ld %ld\n", sinteger, sizeof(sinteger));
    printf("%f %ld\n", floatvar, sizeof(floatvar));
    printf("%f %ld\n", doublevar, sizeof(doublevar));
    

    return 0;
}

