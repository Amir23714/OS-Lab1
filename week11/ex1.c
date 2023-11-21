#include <dirent.h>
#include <stdio.h>

int main() {
    // Open the root directory
    DIR *dir = opendir("./lofsdisk");

    // Read and print the contents of the root directory
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);
}