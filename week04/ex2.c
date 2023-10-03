#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#define ARRAY_LENGTH 120

int main() {
    
    // Initializing two vectors
    int u[ARRAY_LENGTH] = {1, 2, 3};
    int v[ARRAY_LENGTH] = {4, 5, 6};
    
    // Set up randomizer according to current time
    srand(time(NULL));
    
    // Filling vectors
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }
    
    printf("u = [ ");
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        printf("%d,", u[i]);
    }
    printf(" ]\n");
    
    printf("v = [ ");
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        printf("%d,", v[i]);
    }
    printf(" ]\n");
    
    int n;
    printf("Write n = ");
    scanf("%d", &n);
    
    // Initializing array of proccesses
    pid_t processes[n];
    
    if (ARRAY_LENGTH % n != 0){
        printf("ARRAY_LENGTH over n must equal to 0\n");
        return -1;
    }
    
    // Step for each procces
    int step = ARRAY_LENGTH / n;
    
    for (int i = 0; i < n; i++){
        pid_t process = fork();
        
        if (process > 0){
            processes[i] = process;
        } 
        
        if (process == 0){
            
                
            FILE *file;
            file = fopen("temp.txt", "a");
            
            int result = 0;
            
            if (file == NULL){
                printf("Error");
            }
            
            for (int j = i * step; j < step*(i+1); j++){
                if (j == ARRAY_LENGTH){
                    break;
                }
                
                result += u[j] * v[j];
                
            }
            
            fprintf(file, "%d\n", result);
            
            fclose(file);
            
            return 0;
            
        
        }
    }
    
    // Waiting for child proccesses finishing
    int child_status;
    
    for (int i = 0; i < n; i++) {
        
        waitpid(processes[i], &child_status, 0);
        
    }
    
    // Reading results, summing them and printing
    FILE *file;
    file = fopen("temp.txt", "r");
    char line[1024];
    int number;
    
    int result = 0;
    
    for (int i =0; i < n; i++) {
        fgets(line, sizeof(line), file);
        if (sscanf(line, "%d", &number) == 1) {
            result += number;
        }
    }
    
    fclose(file);
    
    printf("Result = %d", result);
    
    return 0;
}
