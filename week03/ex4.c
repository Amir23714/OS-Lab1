#include <stdio.h>
#include <stdlib.h>

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)){
    int element_size = size;
    int sizeOfInt = sizeof(int);
    int sizeOfDouble = sizeof(double);
    
    for (size_t i = 0; i < size*n; i += element_size) {
        initial_value = opr(initial_value, base + i);
    };
    
    return initial_value;
}

void* add_double(const void* a, const void* b){
    double* res = (double*)malloc(sizeof(double));
    if (res != NULL) {
        *res = *(double*)(a) + *(double*)(b);
    }
    return (void*)res;
}

void* multiply_double(const void* a, const void* b){
    double* res = (double*)malloc(sizeof(double));
    if (res != NULL) {
        *res = (*(double*)(a)) * (*(double*)(b));
    }
    return (void*)res;
}

void* max_double(const void* a, const void* b){
    double* res = (double*)malloc(sizeof(double));
    if (res != NULL) {
        if (*(double*)(a) > *(double*)(b)){
            *res = *(double*)(a);
        } else{
            *res = *(double*)(b);
        }
    }
    return (void*)res;
}

void* add_int(const void* a, const void* b){
    int* res = (int*)malloc(sizeof(int));
    if (res != NULL) {
        *res = *(int*)(a) + *(int*)(b);
    }
    return (void*)res;
}

void* multiply_int(const void* a, const void* b){
    int* res = (int*)malloc(sizeof(int));
    if (res != NULL) {
        *res = (*(int*)(a)) * (*(int*)(b));
    }
    return (void*)res;
}

void* max_int(const void* a, const void* b){
    int* res = (int*)malloc(sizeof(int));
    if (res != NULL) {
        if (*(int*)(a) > *(int*)(b)){
            *res = *(int*)(a);
        } else{
            *res = *(int*)(b);
        }
    }
    return (void*)res;
}


int main()
{
    double arr_double[5] = {0.1, 1.25, 2.5, 3.75, 4.99};
    int arr_int[5] = {1, 2, 3, 4, 5};
    
    double* double_base = &arr_double[0];
    int* int_base = &arr_int[0];
    
    printf("Array of doubles\n\nSum = ");
    double* res = (double*)malloc(sizeof(double));
    *res = 0.0;
    res = aggregate((void*)double_base, sizeof(double), 5, (void*)res, add_double);
    printf("%f\n", *res);
    free(res);
    
    printf("Multiplication = ");
    *res = 1.0; 
    res = (double*)aggregate((void*)double_base, sizeof(double), 5, (void*)res, multiply_double);
    printf("%f\n", *res);
    free(res);
    
    printf("Max value of array = ");
    *res = 0.1; 
    res = (double*)aggregate((void*)double_base, sizeof(double), 5, (void*)res, max_double);
    printf("%f\n\n", *res);
    free(res);
    
    
    printf("Array of integers\n\nSum = ");
    int* res_int = (int*)malloc(sizeof(int));
    *res_int = 0.0;
    res_int = aggregate((void*)int_base, sizeof(int), 5, (void*)res_int, add_int);
    printf("%d\n", *res_int);
    free(res);
    
    printf("Multiplication = ");
    *res_int = 1.0; 
    res_int = (int*)aggregate((void*)int_base, sizeof(int), 5, (void*)res_int, multiply_int);
    printf("%d\n", *res_int);
    free(res);
    
    printf("Max value of array = ");
    *res_int = 1; 
    res_int = (int*)aggregate((void*)int_base, sizeof(int), 5, (void*)res_int, max_int);
    printf("%d\n\n", *res_int);
    free(res_int);
    
    return 0;
}
