#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *dir = opendir("./lofsdisk");

    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    struct dirent *entry;

    for (; (entry = readdir(dir)) != NULL;) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}
