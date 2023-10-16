#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>

#define PS_MAX 10

typedef struct {
    int idx;
    int at, bt, rt, wt, ct, tat;
    int burst;
} ProcessData;

int processes_completed = 0;
int running_process = -1;
unsigned total_time = 0;

ProcessData data[PS_MAX];
pid_t ps[PS_MAX];
unsigned data_size;

void read_file(FILE *file){
    data_size = 0;
    char line[100];
    char trash[100];
    fgets(trash, sizeof(trash), file);

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d %d %d", &data[data_size].idx, &data[data_size].at, &data[data_size].bt);
        data[data_size].rt = -1;
        data[data_size].burst = data[data_size].bt;
        data_size++;

    }

};
void resume(pid_t process){
    kill(process, SIGCONT);
};
void suspend(pid_t process){
    kill(process, SIGTSTP);
};
void terminate(pid_t process){
    kill(process, SIGTERM);
};
void create_process(int new_process) {

    for (int i = 0; i < data_size; i++) {
        if (ps[i] != -1){
            suspend(ps[i]);
        }
        
    }

    pid_t process = fork();

    if (process == 0) {

        char new_process_str[20];
        snprintf(new_process_str, sizeof(new_process_str), "%d", new_process);

        char *args[] = {"worker", new_process_str, NULL};
        execvp("./worker", args);
        
        exit(0);
    } else {

        ps[new_process] = process;

        for (int i = 0; i < data_size; i++) {
        if (ps[i] != -1){
            resume(ps[i]);
            };
        

        };
    };
};

ProcessData find_next_process() {
    int next = -1;
    int min_bt = INT_MAX;
    for (int i = 0; i < data_size; i++) {
        if (data[i].rt < 0 && data[i].at <= total_time && data[i].bt < min_bt) {
            min_bt = data[i].bt;
            next = i;
        }
    }

    if (next == -1){
        int min_at = INT_MAX;
        for (int i = 0; i < data_size; i++) {
        if (data[i].rt < 0 && data[i].at < min_at) {
            min_at = data[i].at;
            next = i;
        }
    }
    }

    return data[next];
};
void report() {
    
    for (int i = 0; i < data_size; i++) {
        printf("process %d:\n\tat=%d\n\tbt=%d\n\tct=%d\n\twt=%d\n\ttat=%d\n\trt=%d\n", data[i].idx, data[i].at, data[i].bt, data[i].ct, data[i].wt, data[i].tat, data[i].rt);
    }
    fflush(stdout);
}

void check_burst() {
    if (running_process != -1) {
        data[running_process].burst--;
        if (data[running_process].burst == 0) {
            // Process is finished
            data[running_process].ct = total_time + 1;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            data[running_process].rt = data[running_process].ct - data[running_process].bt - data[running_process].at;
            terminate(ps[running_process]);
    
            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, 0);
            fflush(stdout);
       
            running_process = -1;

            processes_completed +=1;
            
        }
    }
};  
void schedule_handler(int signum) {
    

    if (running_process != -1) {
        check_burst();
    }

    if (processes_completed == data_size){
        printf("Simulation results.....\n");
        fflush(stdout);
        report();

        printf("data size = %d\n", data_size);
        fflush(stdout);

        double avg_wt;
        double sum1 = 0;
        double sum2 = 0;
        for (int i = 0; i < data_size; i++){
            sum1 += data[i].wt;
            sum2 += data[i].tat;
        }
        avg_wt = sum1 / data_size;

        double avg_tat = sum2 / data_size;

        printf("Average results for this run :\n\tavg_wt=%f\n\tavg_tat=%f\n", avg_wt,avg_tat);

        fflush(stdout);
        exit(0);
    }
    
    ProcessData next_process = find_next_process();


    if (next_process.idx != -1) {
        total_time+=1;
        if (running_process == -1){
            if (next_process.at <= total_time){
                running_process = next_process.idx;
                

                printf("Scheduler: Starting Process %d (Remaining Time : %d)\n", next_process.idx, next_process.burst);
                fflush(stdout);
                create_process(running_process);

            } else {
                

                printf("Scheduler: Runtime: %d seconds.\nProcess %d: has not arrived yet.\n", total_time, next_process.idx);
                fflush(stdout);
                return;
        }  
            
        } else{
            
            printf("Scheduler: Runtime: %d seconds.\nScheduler: Process %d is running with %d seconds left\n", total_time, next_process.idx, next_process.burst);
            fflush(stdout);
        }
        
        
    } else {
        
    }
}


int main(int argc, char *argv[]){

    if (argc != 2) {
        return 1;
    }

    const char *filename = argv[1];

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    read_file(file);
    fclose(file);

    for (int i =0; i < data_size; i++){
        ps[i] = -1;
    }

    struct itimerval timer;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("Error setting timer");
        return 1;
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = schedule_handler;
    sigaction(SIGALRM, &sa, NULL);

    while (1) {
        
    }

    return 0;

}
