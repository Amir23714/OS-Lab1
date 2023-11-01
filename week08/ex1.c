#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>

int main() {
    
    FILE *pid_file = fopen("/tmp/ex1.pid", "w");
    fprintf(pid_file, "%d\n", getpid());
    fclose(pid_file);

    int i = 5;
    char password[13] = "pass:";

    int random = open("/dev/random", O_RDONLY);

    while ( i <= 12 ) {
        char c;
        ssize_t bytes_read = read(random, &c, 1);

        if (isprint(c)){
            password[i] = c;
            i++;
        }
    }


    close(random);

    int fd = open("/dev/zero", O_RDWR);

    char *password_on_heap = mmap(NULL, 12, PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0);

    strcpy(password_on_heap, password);
    
    printf("Saved password   %s\n", password);

    while (1) {
      
    }

    munmap(password_on_heap, 13);

    return 0;
}
