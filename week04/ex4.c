#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 256
#define MAX_ARG_SIZE 10

void executeCommand(char *command, char *args[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Unsuccessfull fork creation\n");
        exit(EXIT_FAILURE);
    } 
    
    if (pid == 0) {
        if (execvp(command, args) == -1) {
            perror("Unsuccessfull command execution");
            exit(EXIT_FAILURE);
        }
    } else { 
        
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARG_SIZE];
    char *token;
    printf("Simplistic shell\n");
    while (true) {
        printf("Enter a command to execute ('exit' to leave the simplistic shell): ");
        fgets(input, MAX_INPUT_SIZE, stdin);

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            return 0;
        }

        token = strtok(input, " ");
        int i = 0;
        while (token != NULL && i < MAX_ARG_SIZE - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; 

        bool background = false;
        if (i > 0 && strcmp(args[i - 1], "&") == 0) {
            background = true;
            args[i - 1] = NULL;
        }

        // Execute the command
        if (i > 0) {
            executeCommand(args[0], args);
        }
    }

    return 0;
}