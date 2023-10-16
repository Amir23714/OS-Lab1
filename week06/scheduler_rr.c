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

int QUANTUM;

struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
};

struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*)malloc(
        sizeof(struct Queue));
        
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(
        queue->capacity * sizeof(int));
    return queue;
}

int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}
 
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}

void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}
 
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

typedef struct {
    int idx;
    int at, bt, rt, wt, ct, tat;
    int burst;
    int first_exec;
} ProcessData;

int processes_completed = 0;
int running_process = -1;
unsigned remaining_quantum;
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
    }
};

struct Queue* queue;

void find_next_process() {

    for (int i = 0; i < data_size; i++){
        if (data[i].at == total_time){
            enqueue(queue, i);
        }
    }

};
void report() {
    
    for (int i = 0; i < data_size; i++) {
        printf("process %d:\n\tat=%d\n\tbt=%d\n\tct=%d\n\twt=%d\n\ttat=%d\n\trt=%d\n\tfirst_exec=%d\n", data[i].idx, data[i].at, data[i].bt, data[i].ct, data[i].wt, data[i].tat, data[i].rt, data[i].first_exec);
    }
    fflush(stdout);
}

void check_burst() {
    if (running_process != -1) {
        data[running_process].burst--;
        remaining_quantum--;

        if (data[running_process].burst == 0) {
            // Process is finished
            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            data[running_process].rt = data[running_process].ct - data[running_process].first_exec;

            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, 0);
            fflush(stdout);
            terminate(ps[running_process]);
       
            running_process = -1;

            processes_completed +=1;
            remaining_quantum = QUANTUM;
            
        }

        if (remaining_quantum == 0){
            printf("Scheduler: Freezing Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            suspend(ps[running_process]);
            enqueue(queue, running_process);
            running_process = -1;
            remaining_quantum = QUANTUM;
        }
        
    }
};  
void schedule_handler(int signum) {
    
    find_next_process();

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


    if (isEmpty(queue)){
        total_time+=1;
        return;
    }

    if (running_process == -1) {

        int index = dequeue(queue);
        ProcessData next_process = data[index];
        total_time+=1;

        running_process = index;
        
        if (data[running_process].bt == data[running_process].burst){
            printf("Scheduler: Starting Process %d (Total time : %d Remaining Quantum : %d)\n", next_process.idx, next_process.burst ,remaining_quantum);
            fflush(stdout);

            data[running_process].first_exec = total_time - 1;

            create_process(running_process);

            return;
        } else {
            for (int i = 0; i < data_size; i++) {
                if (ps[i] != -1){
                    suspend(ps[i]);
                }
            }

            printf("Scheduler: Continuing Process %d (Total time : %d Remaining Quantum : %d)\n", next_process.idx, next_process.burst ,remaining_quantum);
            fflush(stdout);
            resume(ps[running_process]);

            return;
        }
        

    } 
    else{
        
        printf("Scheduler: Runtime: %d seconds.\nScheduler: Process %d is running with %d quantum left. Total time left: %d\n", total_time, running_process, remaining_quantum, data[running_process].burst);
        fflush(stdout);
        total_time+=1;
    }
        
        
    
}


int main(int argc, char *argv[]){

    if (argc != 2) {
        return 1;
    }

    queue = createQueue(1000);

    const char *filename = argv[1];

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    read_file(file);
    fclose(file);

    printf("Enter QUANTUM : ");
    scanf("%d", &QUANTUM);
    printf("\nYou entered %d QUANTUM\n", QUANTUM);
    remaining_quantum = QUANTUM;

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
