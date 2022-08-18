#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

int has_run[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
 
void runMe(int* arg) {
    int value = (*arg);
    assert(value >= 0 && value < 5 && "Bad argument passed to 'runMe()!'");

    has_run[value] = 1;

    int* ret = (int*)malloc(sizeof(int));
    *ret = value * value;

    pthread_exit((void*)ret);
}

int run_threads(int n) {
    int sum = 0;

    pthread_t thread_ids[n];
    int ids[n];

    for (int i = 0; i < n; i++) {
        ids[i] = i;
        pthread_create(&thread_ids[i], NULL, (void* (*)(void*))&runMe, (void*)&ids[i]);
    }

    void* vals[n];
    for (int i = 0; i < n; i++) {
        pthread_join(thread_ids[i], &vals[i]);
        sum += *(int*)vals[i];
        free(vals[i]);
        vals[i] = NULL;
    }

    return sum;
}

int main(int argc, char** argv) {

    int sum = run_threads(2);

    int correct = 0;
    for (int i = 0; i < 2; ++i) {
        if (has_run[i]) correct++;
    }

    printf("%d %d\n", correct, sum);

    return 0;
}