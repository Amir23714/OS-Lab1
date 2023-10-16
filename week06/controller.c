#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

void ctrl_c_handler(int signum) {
    int pid = 0;

    char path[30] = "/var/run/agent.pid";

    char buffer[20];

    FILE *pid_file = fopen(path, "r");

    if (pid_file == NULL) {
        perror("Error: No agent found.");
        exit(1);
    }

    printf("Agent found.\n");

    if (fgets(buffer, sizeof(buffer), pid_file) != NULL) {
        pid = atoi(buffer);

    } else {
        printf("Не удалось прочитать PID из файла\n");
        exit(1);
    }

    kill(pid, SIGTERM) == 0;
    printf("Ctrl+C (SIGINT) received. Exiting...\n");
    exit(0);
}

int main() {
    signal(SIGINT, ctrl_c_handler);

    int pid = 0;

    char path[30] = "/var/run/agent.pid";

    char buffer[20];

    FILE *pid_file = fopen(path, "r");

    if (pid_file == NULL) {
        perror("Error: No agent found.");
        return 1;
    }

    printf("Agent found.\n");

    if (fgets(buffer, sizeof(buffer), pid_file) != NULL) {
        pid = atoi(buffer);
        printf("%d\n", pid);
    } else {
        printf("Не удалось прочитать PID из файла\n");
        return 1;
    }

    while(1){
        printf("Choose a command {“read”, “exit”, “stop”, “continue”} to send to the agent\n");
        char input[1024];

        if (scanf("%1023s", input) == 1) {
            if (strcmp(input, "read") == 0) {
                if (kill(pid, SIGUSR1) == 0){
                    printf("Got read command\n");
                };
            } else if (strcmp(input, "exit") == 0) {
                if (kill(pid, SIGUSR2) == 0){
                    printf("Got exit command\n");
                };
                return 0;
            } else if (strcmp(input, "stop") == 0) {
                if (kill(pid, SIGSTOP) == 0){
                    printf("Got stop command\n");
                };
            } else if (strcmp(input, "continue") == 0) {
                if (kill(pid, SIGCONT) == 0){
                    printf("Got continue command\n");
                };
            } else {
                printf("Unknown command: %s\n", input);
            }
        }
    }

    return 0;
}
