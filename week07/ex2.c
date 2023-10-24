#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define FILE_SIZE 500 * 1024 * 1024 // 500 MiB
#define BUFFER_SIZE 4096

int main() {

    int dev_random_file = open("/dev/random", O_RDONLY);

    if (dev_random_file == -1) {
        perror("Error opening /dev/random");
        return 1;
    }

    int text_file = open("text.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (text_file == -1) {
        perror("Error creating text.txt");
        close(dev_random_file);
        return 1;
    }

    char *mmmap = (char *)mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, text_file, 0);

    if (mmmap == MAP_FAILED) {
        perror("Error mapping the file");
        close(dev_random_file);
        close(text_file);
        return 1;
    }

    size_t current_position = 0;
    size_t capital_count = 0;
    char buffer[BUFFER_SIZE];

    while (current_position < FILE_SIZE) {

        ssize_t bytes = read(dev_random_file, buffer, BUFFER_SIZE);

        if (bytes <= 0) {
            break;
        }

        int i = 0;
        while ( i < bytes && current_position < FILE_SIZE) {
            char c;
            
            if (sscanf(&buffer[i], "%c", &c) == 1 && isprint(c)) {
                mmmap[current_position] = c;

                if (isupper(c)) {
                    capital_count++;
                }
                current_position++;

                if (current_position % 1024 == 0 && current_position < FILE_SIZE) {
                    mmmap[current_position] = '\n';
                    current_position++;
                }
            }
            

            i++;
        }
    }

    printf("Total capital letters: %zu\n", capital_count);

    for (size_t i = 0; i < FILE_SIZE; i++) {
        if (isupper(mmmap[i])) {
            mmmap[i] = tolower(mmmap[i]);
        }
    }

    munmap(mmmap, FILE_SIZE);
    close(dev_random_file);
    close(text_file);

    return 0;
}
