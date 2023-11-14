#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <limits.h> 
#include <time.h>

#define PATH_MAX 4096
#define BUF_LEN (10 * (sizeof(struct inotify_event) + PATH_MAX + 1))

char *path;

void print_stat(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        printf("Stat info for %s:\n", path);
        printf("  Size: %ld bytes\n", st.st_size);
        printf("  Permissions: %o\n", st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
        printf("  Last access time: %s", ctime(&st.st_atime));
        printf("  Last modification time: %s\n", ctime(&st.st_mtime));
    }
    fflush(stdout);
}

void print_all_entries_stat(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            char full_path[PATH_MAX];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
            print_stat(full_path);
        }
    }

    closedir(dir);
}

void signal_handler(int signum) {
    if (signum == SIGINT) {
        printf("\nReceived SIGINT. Printing stat info before termination:\n");
        print_all_entries_stat(path);
        exit(0);
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    path = argv[1];

    signal(SIGINT, signal_handler);

    printf("Printing stat info on startup:\n");
    print_all_entries_stat(path);

    int inotify_fd = inotify_init();

    int watch_fd = inotify_add_watch(inotify_fd, path, IN_ALL_EVENTS);

    char buf[BUF_LEN];
    
    ssize_t num_bytes;

    while (true) {
        num_bytes = read(inotify_fd, buf, BUF_LEN);

        struct inotify_event *event = (struct inotify_event *)buf;

        if (event->name != NULL && event->len > 0) {
            char full_path[PATH_MAX];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, event->name);

            if (event->mask & IN_CREATE) {
                printf("%s created in watched directory\n", event->name);
                fflush(stdout);
            } else if (event->mask & IN_DELETE) {
                printf("%s deleted from watched directory\n\n", event->name);
                fflush(stdout);
                continue;
            } else if (event->mask & IN_MODIFY) {
                printf("%s was modified\n", event->name);
                fflush(stdout);
            } else if (event->mask & IN_ATTRIB) {
                printf("Metadata changed\n");
                fflush(stdout);
            } else if (event->mask & IN_ACCESS) {
                printf("%s was accessed\n", event->name);
                fflush(stdout);
            } else if (event->mask & IN_OPEN) {
                printf("%s was opened\n", event->name);
                fflush(stdout);
            } else {
                continue;
            }

            print_stat(full_path);
        }
    }


    close(inotify_fd);
    exit(EXIT_SUCCESS);
}
