#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define MESSAGE_SIZE 1024

int main(int argc, char *argv[]){

    int id = atoi(argv[1]);

    char ids[4];
    
    char directory_name[64]="/tmp/ex1/s";
    sprintf(ids, "%d", id);
    strcat(directory_name,ids);

    int fd_read=open(directory_name,O_RDONLY);

    if (fd_read==-1){
        printf("err read");
        return 1;
    }
    
    char message[MESSAGE_SIZE];
    if (read(fd_read,message,MESSAGE_SIZE)==-1)
        return 2;
    printf("%s",message);
    close(fd_read);
}