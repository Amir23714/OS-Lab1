#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>



struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

struct PTE *pt;
pid_t pager_processID;

void print_pagetable(int num_pages) {
    for(int j = 0; j < num_pages; j++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", j, pt[j].valid, pt[j].frame, pt[j].dirty, pt[j].referenced);
    }
}

void signal_handler(int signum) {
    if (signum == SIGCONT) {
        return;
    }
}

void mkex2dir(){
    struct stat st = {0};

    if (stat("/tmp/ex2", &st) == -1) {
        mkdir("/tmp/ex2", 0700);
    }
}

void handlePageRequest(char mode, int page) {

    switch (pt[page].valid) {

    case true:
        printf("It is a valid page\n");
        if (mode == 'W') {
            printf("It is a write request then set the dirty field\n");
            pt[page].dirty = true;
        }
        break;
    
    default:
        printf("It is not a valid page --> page fault\n");
        printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
        pt[page].referenced = getpid();
        kill(pager_processID, SIGUSR1);
        pause();
        printf("MMU resumed by SIGCONT signal from pager\n");
        break;
    }

}

void mainHandler(char * memory_accesses, int P){
    int i = 0;
    char mode;
    int page;

    while(i < strlen(memory_accesses)) {
        
        mode = memory_accesses[i];
        i++;
        
        page = memory_accesses[i] - '0';
        i += 2; 

        switch (mode) {
            case 'R':
                printf("Read Request for page %d\n", page);
                break;
            case 'W':
                printf("Write Request for page %d\n", page);
                break;
        }

        handlePageRequest(mode, page);

        printf("Page table\n");
        print_pagetable(P);
        printf("-------------------------\n");
    }
}

void mmuTermination() {
    printf("Done all requests.\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    kill(pager_processID, SIGUSR1);
    printf("MMU terminates.\n");
}

int main(int argc, char *argv[]) {

    int P = atoi(argv[1]);
    char *memory_accesses = argv[2];
    pager_processID = (pid_t)atoi(argv[3]);

    signal(SIGCONT, signal_handler);

    mkex2dir();
    
    int file = open("/tmp/ex2/pagetable", O_RDWR);

    pt = mmap(NULL, sizeof(struct PTE) * P, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    printf("-------------------------\n");
    printf("Initialized page table\n");
    print_pagetable(P);
    printf("-------------------------\n");

    mainHandler(memory_accesses, P);

    mmuTermination();

    munmap(pt, sizeof(struct PTE) * P);
    close(file);

    return 0;
}

