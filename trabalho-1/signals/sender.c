#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
  int pid, signal;

  printf("MODO | CÓDIGO\n");
  printf("--------------\n");
  printf("SIGTERM | %d\n", SIGTERM);
  printf("--------------\n");
  printf("SIGUSR1 | %d\n", SIGUSR1);
  printf("--------------\n");
  printf("SIGUSR2 | %d\n", SIGUSR2);
  printf("Para demais códigos consulte: https://man7.org/linux/man-pages/man7/signal.7.html\n");

  printf("\nDigite o PID do processo: \n");
  scanf("%d", &pid);
  printf("\nDigite o código do sinal: \n");
  scanf("%d", &signal);

  int sucess_code = kill(pid, signal);

  if (sucess_code < 0 ) {
    switch (errno) {
      case EINVAL:
        printf("\nSinal inexistente.");
        break;
      case EPERM:
        printf("\nOperação não permitida.");
        break;
      case ESRCH:
        printf("\nProcesso inexistente.");
        break;
      default:
        printf("Erro. Verifique e tente novamente.");
        break;
    }
    return -1;
  } else {
    printf("Sinal enviado ao processo com sucesso.");
    return 1;
  };
}