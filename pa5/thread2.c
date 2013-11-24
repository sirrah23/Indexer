#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sum = 0;
pthread_t *threads;
pthread_mutex_t mutexsum;

void *add(void *num) {
    pthread_mutex_lock(&mutexsum);
    sum += 100;
    printf("Thread %ld incremented sum to %d\n", (long)num, sum);
    pthread_mutex_unlock(&mutexsum);
    pthread_exit(NULL);
}

int main() {
    long i = 0; void *status;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&mutexsum, NULL);

    threads = malloc(sizeof(pthread_t));
    pthread_create(&threads[i], &attr, add, (void *)i);
    do {
        i++;
        threads = realloc(threads, sizeof(pthread_t) * (i+1));
        pthread_create(&threads[i], &attr, add, (void *)i);
    } while(i < 4);
    pthread_attr_destroy(&attr);

    for(i = 0; i < 5; i++)
        pthread_join(threads[i], &status);
    
    pthread_mutex_destroy(&mutexsum);
    printf("sum is %d\n", sum);
    free(threads);
    pthread_exit(NULL);
}
