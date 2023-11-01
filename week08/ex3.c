#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#define MB (1024 * 1024)
#define MEMORY_TO_ALLOCATE (10 * MB)

void print_memory_usage() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        printf("Memory usage: %ld KB\n", usage.ru_maxrss);
    }
}

int main() {
    int seconds = 10;

    while (seconds != 0) {
        char *memory_block = (char *)malloc(MEMORY_TO_ALLOCATE);

        memset(memory_block, 0, MEMORY_TO_ALLOCATE);

        print_memory_usage();

        sleep(1);
        seconds--;
    }

    return 0;
}
