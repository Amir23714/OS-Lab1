#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    
    int n = atoi(argv[1]);
    
    for (int i = 0; i < n; i++){
        pid_t child_pid = fork();
        
        
       
        sleep(5);
        
    }
    
    return 0;
}
