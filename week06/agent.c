#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void sigusr1_handler(int signal_num) {

    FILE *file;
    char line[1024];

    file = fopen("text.txt", "r");

    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(1);
    }
    
    while (fgets(line, sizeof(line), file)) {
        printf("%s\n", line);
        fflush(stdout);
    }

    fclose(file);
}

void sigusr2_handler(int signal_num) {
    printf("Process terminating...\n");
    exit(0);
}

int main() {

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2 ,sigusr2_handler);
    
    char path[30] = "/var/run/agent.pid";
 
    pid_t process = getpid();

    FILE * pid_file = fopen(path, "w");

    if (pid_file == NULL) {
        perror("Ошибка открытия файла agent.pid");
        exit(EXIT_FAILURE);
    };
    
    fprintf(pid_file, "%d\n", process);
    printf("%d", process);
    fclose(pid_file);

    fflush(stdout);

    while (1) {
        sleep(1); 
    }

    return 0;
}
