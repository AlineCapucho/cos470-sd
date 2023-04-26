#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <time.h>

// Função de validação de número primo
int is_prime(int n) {
    int i;
    for (i = 2; i <= n / 2; ++i) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main() {
    // Declaração de variáveis
    int fd[2]; // file descriptor
    // fd[0]: leitura
    // fd[1]: escrita
    pid_t pid;
    int buffer_size = 20;
    char buffer[buffer_size];

    // Criação do pipe
    if (pipe(fd) == -1) {
        printf("Pipe falhou.\n");
        return 1;
    }

    // Realização do fork
    pid = fork();
    if (pid == -1) {
        printf("Fork falhou.\n");
        return 1;
    }

    if (pid > 0) { // Código a ser executado pelo processo pai
        close(fd[0]);
        int n = 0;
        printf("Informe quantos números devem ser gerados: ");
        scanf("%d", &n);
        srand(time(NULL)); // random number generator seed
        int x = 1;
        char y[buffer_size];
        int w;
        for (int i=0; i != n; ++i) {
            x = x + 1 + (rand() % 99); // rand() is in {0, 1, ..., RAND_MAX}
            snprintf(y, buffer_size, "%d", x); // copy int x to char y
            w = write(fd[1], &y, strlen(y)+1);
            if (w == -1) {
                printf("Write i={%d} falhou.\n", i);
            }
            sleep(1);
        }
        w = write(fd[1], "0", strlen("0")+1);
        if (w == -1) {
            printf("Write str(0) falhou.\n");
        }
        close(fd[1]);
        sleep(1);
        return 0;
    } else { // Código a ser executado pelo processo filho
        close(fd[1]);
        bool consuming = true;
        int r;
        while (consuming) {
            r = read(fd[0], buffer, buffer_size);
            if (r == -1) {
                printf("Read falhou.\n");
            }
            int x = atoi(buffer); // Converte valor lido do buffer para int
            if (x == 0) {
                consuming = false;
            } else {
                if (is_prime(x)) {
                    printf("O número %d é primo.\n", x);
                } else {
                    printf("O número %d não é primo.\n", x);
                }
                sleep(1);
            }
        }
        close(fd[0]);
        return 0;
    }
}