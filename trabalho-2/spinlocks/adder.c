#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <time.h>
#include <pthread.h>

// Command to compile: gcc -std=c17 -pthread adder.c -o adder 

int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* routine(void* ptr) {
    pthread_mutex_lock(&mutex);
    sum += 1;
    pthread_mutex_unlock(&mutex);
}

void init_random_arr(signed char* arr, int size) {
    srand(time(NULL)); // random number generator seed
    int x = 1;
    for (int i = 0; i < size; ++i) {
        x = -100 + (rand() % 201); // rand() is in {0, 1, ..., RAND_MAX},
        // therefore x is in {-100, 100}
        arr[i] = x;
    }
}

void init_pthread_arr(pthread_t* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (pthread_create(&arr[i], NULL, &routine, NULL) != 0) {
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
    if (argc != 3) {
        printf("Incorrect number of parameters passed. Ending program.\n");
        return 1;
    }
    else {
        int N = atoi(argv[1]);
        signed char arr[N];
        init_random_arr(arr, N);
        print_arr(arr, N);

        int K = atoi(argv[2]);
        pthread_t th[K];
        init_pthread_arr(th, K);
        join_pthread_arr(th, K);

        printf("The value of sum is: %d\n", sum);
        printf("Ending program.\n");

        return 0;
    }
}