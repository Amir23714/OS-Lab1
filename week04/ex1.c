#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main() {
    pid_t process1, process2;
    struct timeval start, end;
    
    gettimeofday(&start, NULL);
    
    (process1 = fork()) && (process2 = fork()); 
    
    if (process1 == 0) {
        sleep(1);
        printf("Child Process 1:\npid :%d\nppid:%d\n", getpid(), getppid());
    } else if (process2 == 0) {
        sleep(2);
        printf("Child Process 2:\npid :%d\nppid:%d\n", getpid(), getppid());
    } else {
        sleep(3);
        printf("Parent Process:\npid :%d\nppid:%d\n", getpid(), getppid());
    }
    
    gettimeofday(&end, NULL);
    
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    
    printf("Process with pid %d took %f seconds\n\n", getpid(), elapsed_time);

    return 0;
}
