#include <stdint.h>
#include <pthread.h>
#include <stdio.h>

typedef struct {
    pthread_t id;
    pthread_mutex_t* mutex;
    int* sum;
} worker_t;

void* run_summation(void* arg) {
    worker_t* worker = *(worker_t**)arg;

    pthread_mutex_lock(worker->mutex);
    *(worker->sum) += 1;
    pthread_mutex_unlock(worker->mutex);

    return NULL;
}

int main() {
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int sum = 0;

    worker_t workers[16] = {0};
    for (int i = 0; i < 16; i++) {
        worker_t* worker = &workers[i];
        worker->mutex = &mutex;
        worker->sum = &sum;

        pthread_create(&worker->id, NULL, run_summation, &worker);
    }

    for (int i = 0; i < 16; i++)
        pthread_join(workers[i].id, NULL);

    printf("sum: %d\n", sum);

    return 0;
}