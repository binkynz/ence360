#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

typedef struct {
    char* message;
    sem_t write;
    sem_t read;
} entity_t;

void* producer(void* arg) {
    char* words[5] = {"hello", "my", "name", "is", "sean"};

    entity_t* ent = (entity_t*)arg;

    for (char i = 0; i < 5; i++) {
        sem_wait(&ent->write);
        ent->message = words[i];
        sem_post(&ent->read);
    }

    sem_wait(&ent->write);
    ent->message = NULL;
    sem_post(&ent->read);

    return NULL;
}

char* read_ent(entity_t* ent) {
    sem_wait(&ent->read);
    char* message = ent->message;
    ent->message = NULL;
    sem_post(&ent->write);

    return message;
}

int main() {
    entity_t ent = {0};
    sem_init(&ent.write, 0, 1);
    sem_init(&ent.read, 0, 0);

    pthread_t ids[4];
    for (char i = 0; i < 4; i++)
        pthread_create(&ids[i], NULL, producer, &ent);

    char finished = 0;
    while (finished < 4) {
        char* message = read_ent(&ent);
        if (message)
            printf("recieved: %s\n", message);
        else
            finished++;
    }

    for (char i = 0; i < 4; i++)
        pthread_join(ids[i], NULL);

    return 0;
}