// Amir Gubaidullin 06.12.2023 23:29
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_THREADS 100
#define MAX_RESOURCES 100

// Node structure for a queue
struct QueueNode {
    int data;
    struct QueueNode* next;
};

// Queue structure
struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

// Initialize an empty queue
void initializeQueue(struct Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

// Check if the queue is empty
int isEmpty(struct Queue* queue) {
    return (queue->front == NULL);
}

// Add an element to the queue
void enqueue(struct Queue* queue, int data) {

    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = NULL;

    if (isEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Remove and return the front element from the queue
int dequeue(struct Queue* queue) {
    if (isEmpty(queue)) {
        return -1;
    }
    int data = queue->front->data;

    struct QueueNode* temp = queue->front;
    queue->front = queue->front->next;
    free(temp);

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    return data;
}

// Check if a given value is in the queue
bool inQueue(struct Queue* queue, int data) {
    if (queue->front == NULL) {
        return false;
    }

    struct QueueNode* nextNode = queue->front;
    while (nextNode != NULL) {
        if (nextNode->data == data) {
            return true;
        }
        nextNode = nextNode->next;
    }
    return false;
}

// Structure to represent thread arguments
struct ThreadArgs {
    int thread_id;
    int resource_id;
};

// Node structure for a resource
struct ResourceNode;

// Node structure for a thread
struct ThreadNode;

// Graph structure to represent the relationships between threads and resources
struct Graph {
    struct ThreadNode* threads[MAX_THREADS];
    struct ResourceNode* resources[MAX_RESOURCES];
};

// Node structure for a resource
struct ResourceNode {
    int id;
    struct ThreadNode* usedBy;
    bool isAllocated;
};

// Node structure for a thread
struct ThreadNode {
    int id;
    int size;
    struct ResourceNode* waitesFor[MAX_RESOURCES];
    bool isAllocated;
};

// Global variables
int n, m;
struct Graph graph;
struct Queue resourses_queues[MAX_RESOURCES];
pthread_t threads[MAX_THREADS] = {0};
int point = 0;

// Function to handle deadlock occurrence
void deadlockOccur() {
    printf("Deadlock is detected\n");
    fflush(stdout);
    
    for (int i = 0 ; i < m; i++){
        if (graph.resources[i]->isAllocated && graph.resources[i]->usedBy != NULL){
            printf("Mutex %d is locked by Thread %d\n", i, graph.resources[i]->usedBy->id);
            fflush(stdout);
            for (int j = 0 ; j < n; j++){
                if (inQueue(&resourses_queues[i], j)){
                    printf("Mutex %d is requested by Thread %d\n", i, j);
                    fflush(stdout);
                }
            }
        }   
    }

    exit(0);
}

// Depth-first search to detect deadlock
void dfs(bool isThread, int id, bool visitedThreadNodes[], bool visitedResourceNodes[]) {
    
    if (isThread) {
        for (int i = 0; i < graph.threads[id]->size; i++) {
            if (graph.threads[id]->waitesFor[i] != NULL) {
                visitedThreadNodes[id] = true;
                // printf("Visiting thread %d\n", id);
                // fflush(stdout);
                dfs(!isThread, graph.threads[id]->waitesFor[i]->id, visitedThreadNodes, visitedResourceNodes);
            }
        }
        return;
    } else {
        if (visitedResourceNodes[id] == true) {
            deadlockOccur();
        }
        visitedResourceNodes[id] = true;

        if (graph.resources[id]->usedBy != NULL) {
            // printf("Visiting mutex %d\n", id);
            // fflush(stdout);
            dfs(!isThread, graph.resources[id]->usedBy->id, visitedThreadNodes, visitedResourceNodes);
        } else {
            return;
        }
    }
}

// Check for deadlock in the entire graph
void checkDeadlock() {
    for (int i = 0; i < m; i++) {
        if (graph.resources[i]->isAllocated) {
            bool visitedThreadNodes[MAX_THREADS] = {false};
            bool visitedResourceNodes[MAX_RESOURCES] = {false};

            dfs(false, i, visitedThreadNodes, visitedResourceNodes);
        }
    }
    // for (int i = 0 ; i < n; i++){
    //     if (graph.threads[i]->isAllocated) {
    //         bool visitedThreadNodes[MAX_THREADS] = {false};
    //         bool visitedResourceNodes[MAX_RESOURCES] = {false};

    //         dfs(true, i, visitedThreadNodes, visitedResourceNodes);
    //         printf("\n");
    //     }
    // }
}

// Function executed by threads to allocate resources
void* allocateResource(void* args) {
    // Extract thread arguments
    struct ThreadArgs* threadArgs = (struct ThreadArgs*)args;
    int thread_id = threadArgs->thread_id;
    int resource_id = threadArgs->resource_id;

    // Remove the resource from the list of resources thread is waiting for
    for (int i = 0; i < graph.threads[thread_id]->size; i++) {
        if (graph.threads[thread_id]->waitesFor[i] == graph.resources[resource_id]) {
            graph.threads[thread_id]->waitesFor[i] = NULL;
        }
    }

    // Update the resource to be used by the current thread
    graph.resources[resource_id]->usedBy = graph.threads[thread_id];

    // Simulate resource usage with a random sleep duration
    int timeToSleep = rand() % 6;
    
    printf("Thread %d locks mutex %d for %d seconds\n", thread_id, resource_id, timeToSleep);
    fflush(stdout);
    sleep(timeToSleep);

    // Release the resource
    graph.resources[resource_id]->usedBy = NULL;
    printf("Thread %d unlocks mutex %d\n", thread_id, resource_id);
    fflush(stdout);

    // Try to allocate the resource to the next waiting thread
    int nextThread_id = dequeue(&resourses_queues[resource_id]);
    if (nextThread_id == -1) {
        pthread_exit(NULL);
    } else {
        threadArgs->thread_id = nextThread_id;
        checkDeadlock();
        pthread_t myThread;
        if (pthread_create(&myThread, NULL, allocateResource, (void*)threadArgs) != 0) {
            printf("Error occurred\n");
            fflush(stdout);
            pthread_exit(NULL);
        }

        pthread_join(myThread, NULL);
        pthread_exit(NULL);
    }
}

// Process resource requests made by a thread
void processRequests(int thread_id) {
    for (int i = 0; i < m; i++) {
        if (graph.resources[i]->isAllocated && graph.resources[i]->usedBy != NULL && graph.resources[i]->usedBy->id == thread_id) {
            printf("Thread %d currently locks mutex %d\n", thread_id, i);
            fflush(stdout);
        }
    }
}

// Create or update a thread based on resource allocation
void createThread(int thread_id, int resource_id) {
    struct ThreadNode* Thread;

    // If the thread is not allocated, create it
    if (!graph.threads[thread_id]->isAllocated) {
        printf("Thread %d is created\n", thread_id);
        fflush(stdout);

        graph.threads[thread_id]->id = thread_id;
        graph.threads[thread_id]->size = 0;
        graph.threads[thread_id]->isAllocated = true;

        Thread = graph.threads[thread_id];
    } else {
        // If the thread is already allocated, use the existing structure
        Thread = graph.threads[thread_id];
    }

    processRequests(thread_id);

    printf("Thread %d tries to lock mutex %d\n", thread_id, resource_id);
    fflush(stdout);
    
    if (!graph.resources[resource_id]->isAllocated) {
        graph.resources[resource_id]->isAllocated = true;
        graph.resources[resource_id]->id = resource_id;

        struct ThreadArgs* threadArgs = (struct ThreadArgs*)malloc(sizeof(struct ThreadArgs));
        if (threadArgs == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        threadArgs->resource_id = resource_id;
        threadArgs->thread_id = thread_id;

        pthread_t myThread;
        if (pthread_create(&myThread, NULL, allocateResource, (void*)threadArgs) != 0) {
            printf("Error occurred\n");
            fflush(stdout);
            return;
        }

        threads[point] = myThread;
        point += 1;
    } else {
        // If the resource is not available, handle potential deadlock scenarios
        

        if (graph.resources[resource_id]->usedBy != NULL) {
            // Make the thread wait for this resource

            graph.threads[thread_id]->waitesFor[graph.threads[thread_id]->size] = graph.resources[resource_id];
            graph.threads[thread_id]->size += 1;

            

            enqueue(&resourses_queues[resource_id], thread_id);
            checkDeadlock();
        } else {

            struct ThreadArgs* threadArgs = (struct ThreadArgs*)malloc(sizeof(struct ThreadArgs));
            if (threadArgs == NULL) {
                fprintf(stderr, "Memory allocation error\n");
                exit(EXIT_FAILURE);
            }
            threadArgs->resource_id = resource_id;
            threadArgs->thread_id = thread_id;

            pthread_t myThread;
            if (pthread_create(&myThread, NULL, allocateResource, (void*)threadArgs) != 0) {
                printf("Error occurred\n");
                fflush(stdout);
                return;
            }
            threads[point] = myThread;
            point += 1;
        }
    }
}

// Allocate memory for the graph
void allocateGraph() {
    for (int i = 0; i < m; i++) {
        graph.resources[i] = (struct ResourceNode*)malloc(sizeof(struct ResourceNode));
        if (graph.resources[i] == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        graph.resources[i]->isAllocated = false;
    }

    for (int i = 0; i < n; i++) {
        graph.threads[i] = (struct ThreadNode*)malloc(sizeof(struct ThreadNode));
        if (graph.threads[i] == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        graph.threads[i]->isAllocated = false;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <m> <n>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    m = atoi(argv[1]);
    n = atoi(argv[2]);

    if (m <= 0 || n <= 0 || m > MAX_RESOURCES || n > MAX_THREADS) {
        fprintf(stderr, "Invalid values for m or n\n");
        exit(-1);
    }

    FILE* file = fopen("input.txt", "r");

    allocateGraph();

    // Read input file and process thread/resource requests
    char line[100];
    int thread_id, resource_id;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d %d", &thread_id, &resource_id) == 2) {
            createThread(thread_id, resource_id);
            sleep(0.5);
        } else {
            continue;
        }
    }

    fclose(file);

    // Wait for all threads to finish
    for (int i = 0; i < point; i++) {
        if (threads[i] != 0) {
            pthread_join(threads[i], NULL);
        }
    }

    // Print a message indicating no deadlocks occurred
    printf("No deadlocks\n");
    fflush(stdout);

    return 0;
}
