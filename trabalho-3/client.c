#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// as que sao usadas no trabalho de sockets e
// não importei são essas daqui, caso apareça
// estar faltando algo, descomentar elas
// #include <stdbool.h>
// #include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


int sendRequest () {
}

void receiveGrant () {
}

void sendRelease () {
}

void writeResult() {
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Número incorreto de parâmetros passados. Encerrando programa.\n");
        return -1;
    } else {
      // r -> Quantidade de repetições de acesso à região crítica
      // k -> Segundos em que processo deve permanecer na região crítica
      int r = atoi(argv[1]);
      int k = atoi(argv[2]);

      // Declaração de variáveis
      const buffer_size = 2000;
      int client_socket;
      char server_buffer[buffer_size], client_buffer[buffer_size];
      struct sockaddr_in server_address;
      int port = 8080;
      char *ip = "127.0.0.1";

      // Criação do socket
      client_socket = socket(AF_INET, SOCK_STREAM, 0);

      if (client_socket < 0) {
        printf("Erro na criação do socket.\n");
        return -1;
      }
      printf("Socket criado com sucesso.\n");

      // Configuração do client
      server_address.sin_family = AF_INET;
      server_address.sin_addr.s_addr = inet_addr(ip);
      server_address.sin_port = htons(port);

      // Loop de requisições 
      for (int i = 0; i < r; i++) {
        if (sendRequest() < 0) {
          printf("Erro ao enviar request.\n");
          return -1;
        } else {
          return 1;
        }
      }
    }
}