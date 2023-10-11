#include <stdio.h>
#include <unistd.h>

#define MESSAGE_LENGTH 1024

int main(){
    int p[2];

    pid_t process1, process2;

    if (pipe(p) < 0) {
        printf("Error in creating of pipeline\n");
        return -1;
    }

    process1 = fork(); 

    if (process1 == 0) {
        close(p[0]);
        while(1){
            char message[MESSAGE_LENGTH];
            printf("Enter a message: ");
            fgets(message, MESSAGE_LENGTH, stdin);
            write(p[1], message, MESSAGE_LENGTH);
            }

        return 0;
    } else if (process1 > 0){
        char message[MESSAGE_LENGTH];
        read(p[0], message, MESSAGE_LENGTH);
        printf("I am subscriber. Sended message : %s\n", message);

        return 0;
    }

    int status;

    waitpid(process1, &status, 0);
    

    return 0;
}