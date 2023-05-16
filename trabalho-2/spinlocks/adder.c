#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <time.h>
#include <pthread.h>

// Comando para compilar: gcc -std=c17 -pthread adder.c -o adder
// Comando para executar: ./adder (parâmetro 1 - N) (parâmetro 2 - K)

// Criação do spinlock usando variáveis atômicas e a instrução test_and_set
atomic_flag lock = ATOMIC_FLAG_INIT;
void aquire() {
    while (atomic_flag_test_and_set(&lock)) {}
}
void release() {
    atomic_flag_clear(&lock);
}

// Definição da variavel global sum, que armazenará o resultado da soma em múltiplos arrays
// e dos parâmetros a serem passados para a função routine
int sum = 0;
typedef struct {
    int totalItems;
    signed char* firstItem;
} RoutineArgs;

// Rotina a ser executada por cada thread
// Recebe um ponteiro para o primeiro item do array a ser somado naquela thread
// E a quantidade total de números que a thread deve somar
void* routine(void* input) {
    int parcialSum = 0;
    int runs = 0;
    signed char currentItem = ((RoutineArgs*)input)->firstItem;
    while (runs < ((RoutineArgs*)input)->totalItems) {
        currentItem = *(((RoutineArgs*)input)->firstItem + runs);
        parcialSum += currentItem;
        runs += 1;
    }
    aquire();
        sum += parcialSum;
    release();
}

// Cria um array de números aleatórios
void init_random_arr(signed char* arr, int size) {
    srand(time(NULL)); // random number generator seed
    int x = 1;
    for (int i = 0; i < size; ++i) {
        x = -100 + (rand() % 201); // rand() está entre {0, 1, ..., RAND_MAX},
        // logo x está entre {-100, 100}
        arr[i] = x;
    }
}

// Inicia o vetor de threads
// Usa um vetor de structs RoutineArgs para os parâmetros das threads
void init_pthread_arr(pthread_t* arr, int size, int numArrSize, signed char* numArr) {
    RoutineArgs *args[size];

    for (int i = 0; i < size; ++i) {
        args[i] = malloc(sizeof(RoutineArgs));
        int total = (numArrSize/size);
        if (i == size - 1) {
            total = (numArrSize/size) + (numArrSize%size);
        }
        args[i]->firstItem = &numArr[((numArrSize/size) * i)];
        args[i]->totalItems = total;

        if (pthread_create(&arr[i], NULL, &routine, (void *)args[i]) != 0) {
            printf("Criação da thread %d falhou.\n", i);
            exit(1);
        }
    }
}

// Aguarda a finalização de um vetor de threads
void join_pthread_arr(pthread_t* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (pthread_join(arr[i], NULL) != 0) {
            printf("Execução da thread %d falhou.\n", i);
            exit(1);
        }
    }
}

// Executa a soma em uma única thread
int single_thread_sum(signed char* arr, int size) {
    int totalSum = 0;
    for (int i = 0; i < size; ++i) {
        totalSum += arr[i];
    }
    return totalSum;
}

// Printa um array
void print_arr(signed char* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (i > 0 && i % 10 == 0) {
            printf("\n");
        }
        printf("%3d ", arr[i]);
    }
    printf("\n");
}

// Escreve os dados de tempo de execução em função dos parâmetros em um arquivo
void write_runtime_data(int N, int K, double runtime) {
    char filename[] = "Runtimes.txt";
    FILE* ptr;
    ptr = fopen(filename, "a");

    if (ptr == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    fprintf(ptr, "%d", N);
    fprintf(ptr, "%s", " ");
    fprintf(ptr, "%d", K);
    fprintf(ptr, "%s", " ");
    fprintf(ptr, "%f", runtime);
    fprintf(ptr, "%s", "\n");

    fclose(ptr);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Número incorreto de parâmetros passados. Encerrando programa.\n");
        return -1;
    }
    else {
        int N = atoi(argv[1]);
        int K = atoi(argv[2]);
        if (K <= 0 || N <= 0) {
            printf("Os parâmetros K e N não devem ser nulos ou negativos. Encerrando programa.\n");
            exit(1);
        }

        signed char* arr;
        arr = (signed char*)malloc(N * sizeof(signed char));
        init_random_arr(arr, N);
        // print_arr(arr, N);

        struct timespec start, finish;
        double elapsed;
        clock_gettime(CLOCK_MONOTONIC, &start);

        pthread_t th[K];
        init_pthread_arr(th, K, N, arr);
        join_pthread_arr(th, K);

        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

        printf("Execução das threads levou %f segundos.\n", elapsed);

        int singleThreadSum = single_thread_sum(arr, N);

        free(arr);

        printf("O valor da soma com %d threads é: %d\n", K, sum);
        printf("O valor da soma com 1 threads é: %d\n", singleThreadSum);
        if (sum != singleThreadSum){
            printf("O valor das somas não é igual. Encerrando programa.\n");
            exit(1);
        };

        write_runtime_data(N, K, elapsed);

        exit(0);
    }
}