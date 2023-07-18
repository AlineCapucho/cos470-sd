#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// Comando para compilar: gcc -std=c17 client.c -o client 
// Comando para executar: ./client (parâmetro 1 - r) (parâmetro 2 - k)

#define BUFFER_SIZE 1024
#define MESSAGE_SIZE 20

// Declaração de variáveis
int client_socket;
char server_buffer[BUFFER_SIZE], client_buffer[BUFFER_SIZE];
struct sockaddr_in server_address;
int port = 8080;
char *ip = "127.0.0.1";

int requestCode = 1;
int grantCode = 2;
int releaseCode = 3;

char* create_message(int code, int pid) {
  char separator[] = "|";
  char* message_buffer = malloc(MESSAGE_SIZE);
  memset(message_buffer, '\0', MESSAGE_SIZE);

  char requestCode_str[MESSAGE_SIZE];
  snprintf(requestCode_str, MESSAGE_SIZE, "%d", requestCode);
  char pid_str[MESSAGE_SIZE];
  snprintf(pid_str, MESSAGE_SIZE, "%d", pid);
  strcat(message_buffer, requestCode_str);
  strcat(message_buffer, separator);
  strcat(message_buffer, pid_str);
  strcat(message_buffer, separator);
  sprintf((char*)message_buffer, "%s%0*d", message_buffer, MESSAGE_SIZE - strlen(message_buffer) - 1, 0);

  printf("Mensagem criada: %s.\n", message_buffer);
  
  return message_buffer;
}

int send_request (int socket, int pid, int k) {
  memset(client_buffer, '\0', BUFFER_SIZE);
  memset(server_buffer, '\0', BUFFER_SIZE);

  snprintf(client_buffer, BUFFER_SIZE, "%s", create_message(requestCode, pid));

  int client_message_status;

  client_message_status = send(socket, client_buffer, sizeof(client_buffer), 0);
  if (client_message_status == -1) {
    printf("Erro ao enviar request do client.\n");
    return -1;
  }

  int server_message_status = recv(socket, server_buffer, sizeof(server_buffer), 0);
  if (server_message_status == -1) {
    printf("Erro ao receber mensagem do server.\n");
    return -1;
  }

  if(server_buffer[0] == grantCode) {
    printf("Processo acessou a região crítica.\n");

    sleep(k);
  }

  snprintf(client_buffer, BUFFER_SIZE, "%s", create_message(releaseCode, pid));

  client_message_status = send(socket, client_buffer, strlen(client_buffer)+1, 0);
  if (client_message_status == -1) {
    printf("Erro ao enviar release do client.\n");
    return -1;
  }

  return 0;
}

void write_result(int pid) {
  char filename[] = "resultados.txt";
  FILE* ptr;
  ptr = fopen(filename, "a");

  if (ptr == NULL) {
      printf("Erro abrindo o arquivo.\n");
      exit(1);
  }

  char pid_str[BUFFER_SIZE];
  snprintf(pid_str, BUFFER_SIZE, "%d", pid);

  time_t current_time = time(NULL);
  char * current_time_str = ctime(&current_time);
  current_time_str[strlen(current_time_str)-1] = '\0';
  printf("Current Time : %s\n", current_time_str);

  fprintf(ptr, "Hora: %.*s; ", 8, current_time_str + strlen(current_time_str) - 13);
  fprintf(ptr, "Processo: %s\n", pid_str);
  fclose(ptr);
  printf("Arquivo resultados.txt atualizado.\n");
}

int connect_socket() {
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
  return client_socket;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Número incorreto de parâmetros passados. Encerrando programa.\n");
        return -1;
    }

    // r -> Quantidade de repetições de acesso à região crítica
    // k -> Segundos em que processo deve permanecer na região crítica
    int r = atoi(argv[1]);
    int k = atoi(argv[2]);

    int socket = connect_socket();

    if (socket < 0) {
      printf("Erro na conexão do socket.\n");
      return -1;
    }

    printf("Entrando no loop de requisições.\n");
    // Loop de requisições 
    for (int i = 0; i < r; i++) {
      pid_t process_pid = getpid();
      int request = send_request(socket, process_pid, k);

      if (request < 0) {
        printf("Erro ao enviar request.\n");
        return -1;
      }

      write_result(process_pid);
    }

    close(socket);
    return 0;
}