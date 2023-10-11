#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

struct Thread
{
    pthread_t id;
    int i;
    char message[256];
};

void *routine(void *arg)
{
    struct Thread *thread = (struct Thread *)arg;
    sprintf(thread->message, "Hello from thread %d\n", thread->i);
    printf("%s", thread->message);
    return NULL;
}

void createAndJoinThread(struct Thread *thread)
{
    printf("Thread %d is created\n", thread->i);

    pthread_create(&thread->id, NULL, routine, thread);

    pthread_join(thread->id, NULL);
}

void freeThreads(struct Thread *thread_array[], int n)
{
    for (int i = 0; i < n; i++)
    {
        free(thread_array[i]);
    }
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    struct Thread *thread_array[n];

    for (int i = 0; i < n; i++)
    {
        struct Thread *my_thread = malloc(sizeof(struct Thread));
        my_thread->i = i;
        createAndJoinThread(my_thread);
        thread_array[i] = my_thread;
    }

    freeThreads(thread_array, n);
    return 0;
}