#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
int k = 0;
int c = 0;
int n = 0;

int get_number_to_check() {
    int ret = k;
    if (k != n) {
        k++;
    }
    return ret;
}

void increment_primes() {
    c++;
}

bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

void *check_primes(void *arg) {
    int number;

    while (1) {
        pthread_mutex_lock(&global_lock);
        number = get_number_to_check();
        pthread_mutex_unlock(&global_lock);

        if (number >= n) {
            break;
        }

        bool flag = is_prime(number);

        if (flag) {
            pthread_mutex_lock(&global_lock);
            increment_primes();
            pthread_mutex_unlock(&global_lock);
        }
    }
}

void run_threads(int m) {
    pthread_t *threads = malloc(m * sizeof(pthread_t));

    for (int i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, &check_primes, NULL);
    }

    int i = 0;
    while (i < m) {
        pthread_join(threads[i], NULL);
        i++;
    }

    free(threads);
}

int main(int argc, char *argv[]) {
    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    run_threads(m);

    printf("%d\n", c);
    pthread_mutex_destroy(&global_lock);
    exit(EXIT_SUCCESS);
}