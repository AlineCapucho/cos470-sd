#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  // Declaração de variáveis
  int client_socket;
  char server_buffer[2000], client_buffer[2000];
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

  // Envia solicitação de conexão ao server
  if(connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
    printf("Erro ao solicitar conexão.\n");
    return -1;
  }
  printf("Conexão estabelecida com sucesso.\n");

  // Gera inteiros aleatórios
  int n = 0;
  printf("Informe quantos números devem ser gerados: ");
  scanf("%d", &n);

  int buffer_size = 20;
  char buffer[buffer_size];

  int number = 1;
  char char_number;
  int client_message_status, server_message_status;

  for (int i=0; i != n; ++i) {
    memset(client_buffer, '\0', sizeof(client_buffer));
    number = number + 1 + (rand() % 99);
    snprintf(client_buffer, 20, "%d", number);

    printf("Número gerado: %s.\n", &client_buffer);
    client_message_status = send(client_socket, client_buffer, strlen(client_buffer)+1, 0);
    if (client_message_status == -1) {
      printf("Erro ao enviar mensagem do client.\n");
    }

    server_message_status = recv(client_socket, server_buffer, sizeof(server_buffer), 0);
    if(server_message_status < 0){
      printf("Erro ao receber mensagem do server.\n");
      return -1;
    }
    printf("%s", server_buffer);
  }
  
  // Envia 0 após enviar todos os números gerados aleatoriamente
  snprintf(client_buffer, 20, "%d", 0);

  client_message_status = send(client_socket, client_buffer, strlen(client_buffer)+1, 0);
  if (client_message_status == -1) {
    printf("Erro ao enviar mensagem do client.\n");
  }

  // Encerra a conexão:
  close(client_socket);
  
  return 0;

}