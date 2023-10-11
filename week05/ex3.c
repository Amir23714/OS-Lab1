#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

bool is_prime(int n)
{
    if (n <= 1)
        return false;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

int primes_count(int a, int b)
{
    int ret = 0;
    for (int i = a; i < b; i++)
        if (is_prime(i))
            ret++;
    return ret;
}

typedef struct PrimeRequest
{
    int a, b;
} PrimeRequest;

void *prime_counter(void *arg)
{
    PrimeRequest *req = (PrimeRequest *)arg;
    int *count = malloc(sizeof(int));
    *count = primes_count(req->a, req->b);
    return (void *)count;
}

void free_memory(pthread_t *threads, PrimeRequest *prime_requests, void **results)
{
    free(threads);
    free(results);
    free(prime_requests);
}

int count_primes_in_range(int n, int m)
{
    int size = (m + n - 1) / m;

    pthread_t *threads = malloc(m * sizeof(pthread_t));
    PrimeRequest *prime_requests = malloc(m * sizeof(PrimeRequest));
    void **results = malloc(m * sizeof(void *));

    for (int i = 0; i < m; i++)
    {
        prime_requests[i].a = i * size;
        prime_requests[i].b = (prime_requests[i].b = (i + 1) * size) > n ? n : prime_requests[i].b;

        pthread_create(&threads[i], NULL, &prime_counter, &prime_requests[i]);
    }

    for (int i = 0; i < m; i++)
    {
        pthread_join(threads[i], &results[i]);
    }

    int res = 0;
    int i = 0;

    while (i < m)
    {
        res += *(int *)results[i];
        i++;
    }

    free_memory(threads, prime_requests, results);

    return res;
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]), m = atoi(argv[2]);

    int res = count_primes_in_range(n, m);

    printf("%d\n", res);

    return 0;
}
