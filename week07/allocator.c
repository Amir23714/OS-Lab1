#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#define HEAP_SIZE 10000000

static int memory[HEAP_SIZE] = {-1};
static unsigned int next_available = 0;

void allocate_first_fit(unsigned int adrs, int size) {

    int i = 0;
    while (i < HEAP_SIZE) {
        if (memory[i] == -1) {
            int j = i;
            int block_size = 0;
            while (j < HEAP_SIZE && memory[j] == -1) {
                block_size++;
                j++;
                if (block_size >= size) {
                    break;
                }
            }
            if (block_size >= size) {
                for (int k = i; k < i + size; k++) {
                    memory[k] = adrs;
                }
                next_available = j;
                printf("Allocated %d units of memory at address %d.\n", size, adrs);
                return;
            }
        }
        i++;
    }
    printf("Failed to allocate memory for address %d.\n", adrs);
}

void allocateBestFit(unsigned int adrs, int size) {
    int bestFitSize = INT_MAX;
    int bestFitIndex = -1;

    for (int i = 0; i < HEAP_SIZE; i++) {

        int blockSize = 0;

        while (i < HEAP_SIZE && memory[i] == -1) {
            blockSize++;
            i++;
        }

        if (blockSize >= size && blockSize < bestFitSize) {
            bestFitSize = blockSize;
            bestFitIndex = i - blockSize;
        }
    }

    if (bestFitIndex != -1) {

        for (int k = bestFitIndex; k < bestFitIndex + size; k++) {
            memory[k] = adrs;
        }

        printf("Allocated %d units of memory at address %d.\n", size, adrs);
        return;
    } else {
        printf("Failed to allocate memory for address %d.\n", adrs);
    }
}

void allocateWorstFit(unsigned int adrs, int size) {
    int worstFitSize = -1;
    int worstFitIndex = -1;

    for (int i = 0; i < 10000000; i++) {
        int blockSize = 0;

        while (i < 10000000 && memory[i] == -1) {
            blockSize++;
            i++;
        }

        if (blockSize >= size && blockSize > worstFitSize) {
            worstFitSize = blockSize;
            worstFitIndex = i - blockSize;
        }
    }

    if (worstFitIndex != -1) {

        for (int k = worstFitIndex; k < worstFitIndex + size; k++) {
            memory[k] = adrs;
        }

        printf("Allocated %d units of memory at address %d.\n", size, adrs);
        return;
    }
}


void clear(unsigned int adrs) {

    int i = 0;
    while (i < HEAP_SIZE) {
        if (memory[i] == adrs) {
            memory[i] = -1;
        }
        i++;
    }
    printf("Memory for address %d is cleared.\n", adrs);
}

void freeMemory(){
    int i = 0;
    while (i < HEAP_SIZE) {
        memory[i] = -1;
        i++;
    }
}

int main(){

    FILE *file = fopen("queries.txt", "r");
    if (file == NULL) {
        perror("Error occured while openning the file\n");
        return 1;
    }

    char line[100];


    freeMemory();

    clock_t start, end;
    double ff, bf, wf, ff_t, bf_t, wf_t;
    int quieries = 0;

    printf("First fit algorithm\n");
    start = clock();

    while (fgets(line, sizeof(line), file)) {

        size_t line_length = strlen(line);

        if (line_length > 0 && line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }

        char operation[10];
        int arg1, arg2;

        if (sscanf(line, "%s %d %d", operation, &arg1, &arg2) >= 1) {
            if (strcmp(operation, "allocate") == 0) {
                allocate_first_fit(arg1, arg2);
                quieries+=1;
            } else if (strcmp(operation, "clear") == 0) {
                clear(arg1);
                quieries+=1;
            } else if (strcmp(operation, "end") == 0) {

                break;
            }
        }
    }

    end = clock();
    ff = ((double)(end - start)) / CLOCKS_PER_SEC;
    ff_t = (double)(quieries / ff);
    freeMemory();

    fclose(file);

    printf("\nBest fit algorithm\n");

    file = fopen("queries.txt", "r");

    quieries = 0;
    start = clock();

    while (fgets(line, sizeof(line), file)) {

        size_t line_length = strlen(line);

        if (line_length > 0 && line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }

        char operation[10];
        int arg1, arg2;

        if (sscanf(line, "%s %d %d", operation, &arg1, &arg2) >= 1) {
            if (strcmp(operation, "allocate") == 0) {
                allocateBestFit(arg1, arg2);
                quieries+=1;
            } else if (strcmp(operation, "clear") == 0) {
                clear(arg1);
                quieries+=1;
            } else if (strcmp(operation, "end") == 0) {

                break;
            }
        }
    }

    end = clock();
    bf = ((double)(end - start)) / CLOCKS_PER_SEC;
    bf_t = (double)(quieries / bf);

    freeMemory();

    fclose(file);

    printf("\nWorst fit algorithm\n");

    file = fopen("queries.txt", "r");

    quieries = 0;
    start = clock();

    while (fgets(line, sizeof(line), file)) {

        size_t line_length = strlen(line);

        if (line_length > 0 && line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }

        char operation[10];
        int arg1, arg2;

        if (sscanf(line, "%s %d %d", operation, &arg1, &arg2) >= 1) {
            if (strcmp(operation, "allocate") == 0) {
                allocateWorstFit(arg1, arg2);
                quieries+=1;
            } else if (strcmp(operation, "clear") == 0) {
                clear(arg1);
                quieries+=1;
            } else if (strcmp(operation, "end") == 0) {

                break;
            }
        }
    }

    end = clock();
    wf = ((double)(end - start)) / CLOCKS_PER_SEC;
    wf_t = (double)(quieries / wf);

    fclose(file);

    file = fopen("ex1.txt", "w");
    if (file == NULL) {
        perror("Error occured while openning the file\n");
        return 1;
    }

    fprintf(file, "First fit : %f\nThroughput : %f qps\n\nBest fit : %f\nThroughput : %f qps\n\nWorst fit : %f\nThroughput : %f qps\n", ff, ff_t, bf,bf_t, wf, wf_t);

    return 0;
}
