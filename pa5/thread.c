#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *DoMath(void *num) {
    int *x = (int *)num;
    printf("%d + 10 = %d\n", *x, (*x)+10);
    pthread_exit(NULL);
}

int main() {
    pthread_t *threads;
    pthread_attr_t attr;
    void *status;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int i = 0;
    void *y = &i;
    threads = malloc(sizeof(pthread_t));
    printf("creating thread %d\n", i);
    pthread_create(&threads[i], &attr, DoMath, y);
    i++;
    while(i < 5) {
        threads = realloc(threads, sizeof(pthread_t) * (i+1));
        printf("creating thread %d\n", i);
        pthread_create(&threads[i], &attr, DoMath, y);
        i++;
    }
    pthread_attr_destroy(&attr);
    for(i = 0; i < 5; i++) {
        pthread_join(threads[i], &status);
        printf("completed join with thread %d having a status of %ld\n", i, (long)status);
    }

    pthread_exit(NULL);
    free(threads);
}
