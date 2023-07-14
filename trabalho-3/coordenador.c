#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

// Declaração de variáveis
int buffer_size = 2000;
int server_socket, client_socket, client_size;
char server_buffer[buffer_size], client_buffer[buffer_size];
struct sockaddr_in server_address, client_address;
int port = 8080;
char *ip = "127.0.0.1";

int message_type_size = 8;

void write_log(int pid, char* message) {
    char filename[] = "log.txt";
    FILE* ptr;
    ptr = fopen(filename, "a");

    if (ptr == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char pid_str[buffer_size];
    snprintf(pid_str, buffer_size, "%d", pid); // copy int pid to char pid_str

    char message_type[message_type_size];
    if (message == "1") {
        strcpy(message_type, "REQUEST");
    } else if (message == "2") {
        strcpy(message_type, "GRANT");
    } else if (message == "3") {
        strcpy(message_type, "RELEASE");
    }

    time_t current_time = time(NULL);
    char * current_time_str = ctime(&current_time);
    current_time_str[strlen(current_time_str)-1] = '\0';
    printf("Current Time : %s\n", current_time_str);

    fprintf(ptr, "Hora: %.*s; ", 8, current_time_str + strlen(current_time_str) - 13);
    fprintf(ptr, "Mensagem: %s; ", message_type);
    fprintf(ptr, "Processo: %s\n", pid_str);
    fclose(ptr);
}

int main() {
    // Criação do socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        printf("Erro na criação do socket.\n");
        return -1;
    }
    printf("Socket criado com sucesso.\n");

    // Configuração do server
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);

    // Bind do server na porta 8080
    if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address))<0){
        printf("Erro no bind.\n");
        return -1;
    }
    printf("Bind feito com sucesso.\n");

    // Aguarda e recebe conexões do client
    if(listen(server_socket, 1) < 0){
        printf("Error ao aguardar conexão.\n");
        return -1;
    }
    printf("Aguardando conexão.\n");

    while(1) {
        client_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_size);
        printf("Conexão estabelecida com sucesso.\n");

        bool connection_open = true;
        int client_message_status, server_message_status;
        while(connection_open) {
            // // Limpeza do buffer
            memset(server_buffer, '\0', sizeof(server_buffer));
            memset(client_buffer, '\0', sizeof(client_buffer));

            // Recebe mensagem do cliente
            client_message_status = recv(client_socket, client_buffer, sizeof(client_buffer), 0);
            if (client_message_status == -1) {
                printf("Erro ao receber mensagem do client.\n");
                return -1;
            }

            // Valida o número recebido pelo cliente
            // Realiza a ação necessária dependendo do número recebido
            int received_number = atoi(client_buffer);

            if (received_number == 0) {
                connection_open = false;
                close(client_socket);
                printf("Conexão encerrada.\n");
            } else {
                if (is_prime(received_number)) {
                strcpy(server_buffer,  "O número é primo.\n");
                }
                else {
                strcpy(server_buffer, "O número não é primo.\n");
                }
                server_message_status = send(client_socket, server_buffer, sizeof(server_buffer), 0);
                if (server_message_status == -1) {
                printf("Erro ao enviar mensagem do server.\n");
                }
            }
        };
    }
  return 0;
}