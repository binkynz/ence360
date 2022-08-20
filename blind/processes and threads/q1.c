#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

int has_run[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void runMe(int *arg) {
  int value = (*arg);
  assert(value >= 0 && value < 5 && "Bad argument passed to 'runMe()!'");
  
  has_run[value] = 1;
  
  int *ret = (int*)malloc(sizeof(int));
  *ret = value * value; 

  pthread_exit((void*)ret);
}

int run_threads(int n) {
    int sum = 0;

    pthread_t ids[n];
    int args[n];
    for (int i = 0; i < n; i++) {
        args[i] = i;
        pthread_create(&ids[i], NULL, (void*)&runMe, &args[i]);
    }

    void* rc;
    for (int i = 0; i < n; i++) {
        pthread_join(ids[i], &rc);
        sum += *(int*)rc;
        free(rc);
    }

    return sum;
}

int main (int argc, char **argv) { 
  
  int sum = run_threads(5);

  int correct = 0;
  for(int i = 0; i < 5; ++i) {
    if(has_run[i]) correct++;
  }

  printf("%d %d", correct, sum);

  return 0;
}