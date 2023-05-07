#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// Command to compile: gcc -std=c17 -pthread producer_consumer.c -o producer_consumer 

sem_t mutex;
sem_t empty;
sem_t full;

int max = (int) pow(10, 7);

int* buffer;
int count = 0;

void* producer(void* ptr) {
    srand(time(NULL)); // random number generator seed
    while (1) {
        int x = 1 + rand() % max;
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[count] = x;
        count++;
        sem_post(&mutex);
        sem_post(&full);
    }
}

void* consumer(void* ptr) {
    int y = -1;
    while (1) {
        sem_wait(&full);
        sem_wait(&mutex);
        y = buffer[count-1];
        count--;
        sem_post(&mutex);
        sem_post(&empty);
        printf("Consuming %d\n", y);
    }
}

void init_producers_threads(pthread_t* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (pthread_create(&arr[i], NULL, &producer, NULL) != 0) {
            printf("Creation of thread %d failed.\n", i);
            exit(1);
        }
    }
}

void init_consumers_threads(pthread_t* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (pthread_create(&arr[i], NULL, &consumer, NULL) != 0) {
            printf("Creation of thread %d failed.\n", i);
            exit(1);
        }
    }
}

void join_pthread_arr(pthread_t* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (pthread_join(arr[i], NULL) != 0) {
            printf("Execution of thread %d failed.\n", i);
            exit(1);
        }
    }
}

void print_arr(signed char* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (i > 0 && i % 10 == 0) {
            printf("\n");
        }
        printf("%3d ", arr[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Incorrect number of parameters passed. Ending program.\n");
        return 1;
    }
    else {
        int N = atoi(argv[1]);
        int Np = atoi(argv[2]);
        int Nc = atoi(argv[3]);

        buffer = malloc(N * sizeof(int));
        sem_init(&mutex, 0, 1);
        sem_init(&empty, 0, N);
        sem_init(&full, 0, 0);

        pthread_t producers[Np];
        pthread_t consumers[Nc];
        init_producers_threads(producers, Np);
        init_consumers_threads(consumers, Nc);
        join_pthread_arr(producers, Np);
        join_pthread_arr(consumers, Nc);

        free(buffer);
        sem_destroy(&mutex);
        sem_destroy(&empty);
        sem_destroy(&full);

        printf("Ending program.\n");
        return 0;
    }
}