#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define PATH_MAX 4096

char *path;

void print_stat(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        printf("Stat info for %s:\n", path);
        printf("  Size: %ld bytes\n", st.st_size);
        printf("  Permissions: %o\n", st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
        printf("  Last access time: %s", ctime(&st.st_atime));
        printf("  Last modification time: %s", ctime(&st.st_mtime));
        fflush(stdout);
    }
}


void find_all_hlinks(const char *source) {
    struct stat source_stat;

    stat(source, &source_stat);

    printf("Hard links for file %s with inode %lu:\n", source, (unsigned long)source_stat.st_ino);

    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);

    while ((entry = readdir(dir)) != NULL) {
        char file_path[PATH_MAX];
        snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

        struct stat st;

        stat(file_path, &st);

        if (S_ISREG(st.st_mode) && st.st_ino == source_stat.st_ino && strcmp(file_path, source) != 0) {
            char abs_path[PATH_MAX];
            realpath(file_path, abs_path);
            printf("Inode: %lu, Path: %s\n", (unsigned long)st.st_ino, abs_path);
        }
    }

    closedir(dir);
}

void unlink_all(const char *source) {
    struct stat source_stat;

    stat(source, &source_stat);

    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);

    while ((entry = readdir(dir)) != NULL) {
        char file_path[PATH_MAX];
        snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

        struct stat st;

        if (lstat(file_path, &st) == 0) {
            if (S_ISREG(st.st_mode) && st.st_ino == source_stat.st_ino && strcmp(file_path, source) != 0) {
                unlink(file_path);
                sleep(1);
            }
        }
    }

    closedir(dir);
}


void create_sym_link(const char *source, const char *link) {
    symlink(source, link);
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    path = argv[1];

    char source[PATH_MAX];
    snprintf(source, sizeof(source), "%s/myfile1.txt", path);

    char link1[PATH_MAX];
    snprintf(link1, sizeof(link1), "%s/myfile11.txt", path);

    char link2[PATH_MAX];
    snprintf(link2, sizeof(link2), "%s/myfile12.txt", path);

    FILE *file1 = fopen(source, "w");

    if (file1) {
        fprintf(file1, "Hello world.");
        fclose(file1);

        sleep(1);

        link(source, link1);

        sleep(1);

        link(source, link2);

        sleep(1);

        find_all_hlinks(source);

        char new_path[PATH_MAX];
        snprintf(new_path, sizeof(new_path), "/tmp/myfile1.txt");
        rename(source, new_path);

        sleep(1);

        FILE *file11 = fopen(link1, "w");
        if (file11) {
            fprintf(file11, "Modified content of myfile11.txt");
            fclose(file11);
        }

        sleep(1);

        char link3[PATH_MAX];
        snprintf(link3, sizeof(link3), "%s/myfile13.txt", path);

        create_sym_link("/tmp/myfile1.txt", link3);

        sleep(1);

        FILE *file_tmp = fopen("/tmp/myfile1.txt", "w");
        if (file_tmp) {
            fprintf(file_tmp, "Modified content of /tmp/myfile1.txt");
            fclose(file_tmp);
        }

        sleep(1);

        unlink_all(link1);

        sleep(1);

        print_stat(link1);
    }

    return 0;
}
