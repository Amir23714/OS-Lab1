#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

#define PAGE_SIZE 8

struct PTE {

    bool valid;
    int frame;
    bool dirty;
    int referenced;

};

struct PTE *pt;

int P;
int F;

char **RAM;
char **DISK;

int availableFrames;
int diskAccesses;

void print_RAM() {
    printf("RAM array\n");
    for (int i = 0; i <= F-1; i++) {
        printf("Frame %d ---> %s\n", i, RAM[i]);
    }
}

void print_DISK() {
    printf("Disk array\n");
    for (int i = 0; i <= P -1; i++) {
        printf("Page %d ----> %s\n", i, DISK[i]);
    }
}

void signal_handler(int signum) {

    if (signum == SIGUSR1) {

        printf("-------------------------\n");
        printf("A disk access request from MMU Process (pid=%d)\n", getpid());

        int i = 0;
        while (i <= P - 1) {

            if (pt[i].referenced == 0) {
                i++;
                continue;

            } else {

                printf("Page %d is referenced\n", i);
                int frame_id;

                switch(availableFrames){
                    case 0:
                        printf("We do not have free frames in RAM\n");

                        unsigned int victim = (rand()*rand()) % F;

                        bool dirty = false;
                        if (pt[victim].dirty) {
                            strcpy(DISK[victim], RAM[pt[victim].frame]);
                            dirty = true;
                        }

                        printf("Chose a random victim page %d\n", victim);
                        printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, pt[victim].frame);

                        pt[victim].valid = false;
                        pt[victim].dirty = false;
                        pt[victim].referenced = 0;

                        pt[i].frame = pt[victim].frame;

                        pt[i].valid = true;
                        strcpy(RAM[pt[i].frame], DISK[i]);

                        i++;

                        if (dirty){
                            diskAccesses+=2;
                        } else {
                            diskAccesses++;
                        }
                        break;

                    default:

                        frame_id = F - availableFrames;
                        pt[i].frame = frame_id;
                        pt[i].valid = true;

                        strcpy(RAM[frame_id], DISK[i]);
                        
                        printf("We can allocate it to free frame %d\n", frame_id);
                        printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, frame_id);

                        i++;
                        availableFrames--;
                        diskAccesses++;
                        break;

                }

                print_RAM();
                printf("disk accesses is %d so far\n", diskAccesses);

                pt[i].dirty = false;
                pt[i].referenced = 0;

                kill(pt[i].referenced, SIGCONT);
                printf("Resume MMU process\n");
                printf("-------------------------\n");
                return;
            }
        }

        printf("%d disk accesses in total\n", diskAccesses);
        printf("Pager is terminated\n");
        munmap(pt, sizeof(struct PTE) * P);
        unlink("/tmp/ex2/pagetable");

        i = 0;
        while (i <= F-1){
            free(RAM[i]);
            i++;
        }

        i=0;
        while (i <= P-1){
            free(DISK[i]);
            i++;
        }
        free(RAM);
        free(DISK);
        exit(0);

    }
}

void RD_allocate(){
    RAM = (char **)malloc(F * sizeof(char *));
    DISK = (char **)malloc(P * sizeof(char *));

    for (int i = 0; i <= F-1; i++) {
        RAM[i] = (char *)malloc(PAGE_SIZE * sizeof(char));
    }

    for (int i = 0; i <= P-1; i++) {
        DISK[i] = (char *)malloc(PAGE_SIZE * sizeof(char));
    }

    int file = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, 0666);
    ftruncate(file, sizeof(struct PTE) * P);
    pt = mmap(NULL, sizeof(struct PTE) * P, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

}

void diskFill(){
    int random = open("/dev/random", O_RDONLY);
    
    for (int i = 0; i <= P-1; i++) {
        for (int j = 0; j <= PAGE_SIZE - 2; j++) {

            while (true){
                char c;
                ssize_t bytes_read = read(random, &c, 1);

                if (isprint(c)){
                    DISK[i][j] = c;
                    break;
                }    
            }
        }

        DISK[i][PAGE_SIZE - 1] = '\0';
        pt[i].frame = -1;
    }
}

void initialize(){

    printf("-------------------------\n");
    printf("Initialized page table\n");

    for (int i = 0; i <= P-1; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, pt[i].valid, pt[i].frame, pt[i].dirty, pt[i].referenced);
    }

    printf("-------------------------\n");
    printf("Initialized RAM\n");
    print_RAM();
    printf("-------------------------\n");
    printf("Initialized disk\n");
    print_DISK();
    printf("-------------------------\n");

    printf("Pager initialized. PID: %d\n", getpid());
}

int main(int argc, char *argv[]) {

    P = atoi(argv[1]);
    F = atoi(argv[2]);
    availableFrames = F;
    diskAccesses = 0;

    signal(SIGUSR1, signal_handler);

    RD_allocate();

    diskFill();

    initialize();

    while (true) {
        sleep(1);
    }

    wait(NULL);

    return 0;
}

