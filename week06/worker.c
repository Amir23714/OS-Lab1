#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define TRIBASE 1000000

pid_t pid;
int count = 0;
bool isRunning = true;

int process_idx;

long tris;

bool is_triangular(int n) {
    for (int i = 1; i <= n; i++) {
        if (i * (i + 1) == 2 * n) {
            return true;
        }
    }
    return false;
}

long big_n() {
    time_t t;
    long n = 0;
    srand((unsigned)time(&t));
    while (n < TRIBASE)
        n += rand();
    return n % TRIBASE;
}

void signal_handler(int signum) {
    if (signum == SIGTSTP) {
        isRunning = false;
    } else if (signum == SIGCONT) {
        isRunning = true;
    } else if (signum == SIGTERM) {
        printf("Process %d (PID=<%d>): count of triangulars found so far is %d\n", process_idx, getpid(), count);
        fflush(stdout);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <idx>\n", argv[0]);
        return 1;
    }
    
    process_idx = atoi(argv[1]);

    printf("Process %d (PID=<%d>): has been started\n", process_idx, getpid());
    fflush(stdout);

    signal(SIGTERM, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGCONT, signal_handler);

    long n = big_n();

    printf("Process %d (PID=<%d>): will find the next triangular number from (%ld, inf)\n", process_idx, getpid(), n);
    fflush(stdout);

    while (true) {
        if (isRunning) {
            if (is_triangular(n)) {
                printf("Process %d (PID=<%d>): I found this triangular number %ld\n", process_idx, getpid(), n);
                fflush(stdout);
                count++;
                tris = n;
            }

            n++;
        }
        
    }

    return 0;
}
