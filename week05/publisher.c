#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#define MESSAGE_LENGTH 1024

int main(int argc, char *argv[]){
    
    int n = atoi(argv[1]);

    char message[MESSAGE_LENGTH];
    int c;

    printf("Enter a message: ");
    fgets(message, MESSAGE_LENGTH, stdin);

    pid_t parent_id = getpid();

    pid_t processes[n];

    int fd[n][2];
    
    for (int i = 0; i < n; i++ ){
        pid_t process = fork();
        processes[i]=process;

        if (process == 0)
            break;
            
    }

    for (int i = 0 ;i < n; i++){
        if (processes[i]==0){

            char directory_name[64]="/tmp/ex1";
            char fifo_name[8]="s";
            char fifo_number[2];

            sprintf(fifo_number, "%d", i+1);
            strcat(fifo_name,fifo_number);

            if (mkdir(directory_name, 0777) == -1) {
                if (errno!=EEXIST){
                    perror("mkdir");
                    exit(EXIT_FAILURE);
                }
            }

            if (chdir(directory_name) == -1) {
                perror("chdir");
                exit(EXIT_FAILURE);
            }

            if (mkfifo(fifo_name, 0666) == -1) {
                if (errno!=EEXIST) {
                    perror("mkfifo");
                    exit(EXIT_FAILURE);
                }
            }

            strcat(directory_name,"/");
            strcat(directory_name,fifo_name);

            int fd_write=open(directory_name,O_WRONLY);

            if (fd_write==-1) {
                printf("err");
                return 1;
            }

            if (write(fd_write,message,MESSAGE_LENGTH)==-1)
                return 1;

            printf("Written to %s successfully!\n", directory_name);
            close(fd_write);

            return 0;
            }
        }

        int status;

        for (int i = 0; i < n; i++){
            waitpid(processes[i], &status, 0);
        }

        return 0;

    }
    