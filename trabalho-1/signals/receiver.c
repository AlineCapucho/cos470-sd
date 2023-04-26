#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void signal_handler (int userSignal) {
  switch(userSignal) {
    case SIGUSR1:
      printf("Sinal USR1 recebido com sucesso. \n");
      break;
    case SIGUSR2:
      printf("Sinal USR2 recebido com sucesso. \n");
      break;
    case SIGTERM:
      printf("Sinal TERM recebido com sucesso. O programa será finalizado.\n");
      exit(0);
  };
  return;
}

int main() {
  int wait_mode;

  signal(SIGUSR1, signal_handler);
  signal(SIGUSR2, signal_handler);
  signal(SIGTERM, signal_handler);

  printf("MODO DE ESPERA | CÓDIGO\n");
  printf("--------------\n");
  printf("Busy wait | 0\n");
  printf("--------------\n");
  printf("Blocking wait | 1\n");

  printf("Escolha o modo de espera: \n");
  scanf("%d", &wait_mode);

  switch (wait_mode) {
    case 0:
      while(1) {
        sleep(1);
      }
    case 1:
      while(1) {
        pause();
      }
    default:
      printf("Modo de espera inválido.\n");
      printf("Verifique e tente novamente.\n");
      return -1;
  };
}