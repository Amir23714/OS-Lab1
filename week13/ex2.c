// Amir Gubaidullin 06.12.2023 23:29
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_PROCESSES 100
#define MAX_RESOURCES 100

int processes, resourses;

int E[MAX_RESOURCES];
int A[MAX_RESOURCES];

int C[MAX_PROCESSES][MAX_RESOURCES];
int R[MAX_PROCESSES][MAX_RESOURCES];

void readInput(){
    FILE* file = fopen("input.txt", "r");

    char line[100];
    
    for (int i = 0; i < resourses; i++) {
        if (fscanf(file, "%d", &E[i]) != 1) {
            fprintf(stderr, "Error reading from file\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < resourses; i++) {
        if (fscanf(file, "%d", &A[i]) != 1) {
            fprintf(stderr, "Error reading from file\n");
            exit(EXIT_FAILURE);
        }
    }


    for (int i = 0; i < processes; i++){
        for (int j =0; j < resourses ; j ++){
            if (fscanf(file, "%d", &C[i][j]) != 1) {
                fprintf(stderr, "Error reading from file\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    for (int i = 0; i < processes; i++){
        for (int j =0; j < resourses ; j ++){
            if (fscanf(file, "%d", &R[i][j]) != 1) {
                fprintf(stderr, "Error reading from file\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

bool mark_of_processes[MAX_PROCESSES] = {false};

bool checkResourseRequest(int process_index){

    for (int i = 0 ; i < resourses; i++){
        if (R[process_index][i] > A[i]){
            return false;
        }
    }
    return true;
}

void releaseResoursesOfProcess(int process_index){
    for (int i = 0 ; i < resourses; i++){
        A[i] += C[process_index][i];
    }
}

void checkUnmarkedProcesses(){
    bool flag = false;

    for (int i = 0 ; i < processes; i++){
        if (mark_of_processes[i] == false){
            printf("Process %d is deadlocked\n", i);
            flag = true;
        }
    }

    if (flag){
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <processes> <resourses>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    processes = atoi(argv[1]);
    resourses = atoi(argv[2]);

    if (processes <= 0 || resourses <= 0 || resourses > MAX_RESOURCES || processes > MAX_PROCESSES) {
        fprintf(stderr, "Invalid values for m or n\n");
        exit(-1);
    }

    readInput();

    for (int i = 0 ; i < processes; i++){
        if (mark_of_processes[i] == false && checkResourseRequest(i)){
            mark_of_processes[i] = true;
            releaseResoursesOfProcess(i);
        }
    }

    checkUnmarkedProcesses();
    
    printf("No deadlock is detected\n");

    return 0;
}
