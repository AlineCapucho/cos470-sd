#include <stdlib.h>
#include <stdio.h>
// #include <signal.h>
#include <unistd.h>
#include <sys/types.h>
// #include <sys/wait.h>
#include <stdbool.h>
#include <string.h>

int is_prime(int n) {
    int i;
    for (i = 2; i <= n / 2; ++i) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main() {
    int maxBuffer = 20;
    // int pid;
    pid_t pid;
    int fd[2];
    char buffer[maxBuffer];

    if (pipe(fd) == -1) {
        printf("Pipe falhou.\n");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        printf("Fork falhou.\n");
        return 1;
    }

    if (pid > 0) {
        close(fd[0]);
        int n = 0;
        printf("Informe quantos números devem ser gerados: ");
        scanf("%d", &n);
        srand(12);
        int x = 1;
        // char y[20];
        write(fd[1], &x, sizeof(x));
        for (int i=1; i != n; ++i) {
            x = x + rand() % 99;
            char y[20];
            snprintf(y, 20, "%d", x);
            write(fd[1], &y, strlen(y)+1);
        }
        write(fd[1], "0", strlen("0")+1);
        close(fd[1]);
        return 0;
    } else {
        close(fd[1]);
        bool consuming = true;
        while (consuming) {
            read(fd[0], buffer, maxBuffer);
            int x = atoi(buffer);
            if (x == 0) {
                consuming = false;
            } else {
                if (is_prime(x)) {
                    printf("O número %d é primo.\n", x);
                } else {
                    printf("O número %d não é primo.\n", x);
                }
            }
        }
        close(fd[0]);
        return 0;
    }
}