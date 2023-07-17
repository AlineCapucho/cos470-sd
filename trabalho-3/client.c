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

#define BUFFER_SIZE 2000
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

char* createMessage(int code, int pid) {
  printf("Criando mensagem.\n");
  char separator[] = "|";
  char* messageBuffer = malloc(MESSAGE_SIZE);
  memset(messageBuffer, '\0', MESSAGE_SIZE);

  // snprintf(
  //   messageBuffer, 
  //   BUFFER_SIZE, 
  //   "%d%c%d%c", 
  //   requestCode,
  //   separator,
  //   pid,
  //   separator
  // );

  char requestCode_str[MESSAGE_SIZE];
  snprintf(requestCode_str, MESSAGE_SIZE, "%d", requestCode); // copy int x to char y
  printf("Point 0.\n");
  char pid_str[MESSAGE_SIZE];
  snprintf(pid_str, MESSAGE_SIZE, "%d", pid); // copy int x to char y
  printf("Point 1.\n");
  strcat(messageBuffer, requestCode_str);
  printf("Point 2.\n");
  strcat(messageBuffer, separator);
  printf("Point 3.\n");
  strcat(messageBuffer, pid_str);
  printf("Point 4.\n");
  strcat(messageBuffer, separator);
  printf("Point 5.\n");
  sprintf((char*)messageBuffer,"%s%0*d", messageBuffer, MESSAGE_SIZE - strlen(messageBuffer), 0);
  printf("Point 6.\n");

  printf("Mensagem criada.\n");
  printf("%s\n", messageBuffer);
  
  return messageBuffer;
}

int sendRequest (int socket, int pid, int k) {
  printf("Enviando mensagem.\n");
  memset(client_buffer, '\0', BUFFER_SIZE);
  memset(server_buffer, '\0', BUFFER_SIZE);

  // criar mensagem com 
  // o código do request + separadores + padding
  snprintf(
    client_buffer,
    BUFFER_SIZE,
    "%s",
    createMessage(requestCode, pid)
  );

  int client_message_status;

  // tentar enviar mensagem ao servidor
  client_message_status = send(socket, client_buffer, sizeof(client_buffer), 0);
  if (client_message_status == -1) {
    printf("Erro ao enviar request do client.\n");
    return -1;
  }
  printf("Mensagem enviada.\n");

  int server_message_status = recv(socket, server_buffer, sizeof(server_buffer), 0);

  if (server_message_status == -1) {
    printf("Erro ao receber mensagem do server.\n");
    return -1;
  }

  while(server_buffer[0] != grantCode) {
    printf("Processo tentando acessar a região crítica...\n");
    sleep(1);
  }
  printf("Processo acessou a região crítica.\n");

  sleep(k);

  snprintf(
    client_buffer,
    BUFFER_SIZE,
    "%s",
    createMessage(releaseCode, pid)
  );

  client_message_status = send(socket, client_buffer, strlen(client_buffer)+1, 0);

  if (client_message_status == -1) {
    printf("Erro ao enviar release do client.\n");
    return -1;
  }

  return 0;
}

void writeResult(int pid) {
  printf("Escrevendo resultado.\n");
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
  printf("Resultado escrito.\n");
}

int connectSocket() {
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

    int socket = connectSocket();

    if (socket < 0) {
      printf("Erro na conexão do socket.\n");
      return -1;
    }

    printf("Entrando no loop de requisições.\n");
    // Loop de requisições 
    for (int i = 0; i < r; i++) {
      pid_t process_pid = getpid();
      int request = sendRequest(socket, process_pid, k);

      if (request < 0) {
        printf("Erro ao enviar request.\n");
        return -1;
      }

      writeResult(process_pid);
    }

    close(socket);
    return 0;
}