#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue.c"

#define BUFFER_SIZE 2000

// Declaração de variáveis
int server_socket, client_socket, client_size;
char server_buffer[BUFFER_SIZE], client_buffer[BUFFER_SIZE];
struct sockaddr_in server_address, client_address;
int port = 8080;
char *ip = "127.0.0.1";
int max_connections = 1;
int message_type_size = 8;
int ongoing_connections = 0;
int n;
int r;
int k;

// Semáforo para coordenação e sincronização
sem_t mutex;

// Inicializando fila de sockets
node_t *socket_queue = NULL;

void write_log(int pid, char* message) {
    char filename[] = "log.txt";
    FILE* ptr;
    ptr = fopen(filename, "a");

    if (ptr == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char pid_str[BUFFER_SIZE];
    snprintf(pid_str, BUFFER_SIZE, "%d", pid); // copy int pid to char pid_str

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

void handle_message(int client_socket) {
    int client_message_status;
    int pid;

    // // Limpeza do buffer
    memset(server_buffer, '\0', sizeof(server_buffer));
    memset(client_buffer, '\0', sizeof(client_buffer));

    // Recebe mensagem do cliente
    client_message_status = recv(client_socket, client_buffer, sizeof(client_buffer), 0);
    if (client_message_status == -1) {
        printf("Erro ao receber mensagem do client.\n");
        exit(1);
    }

    pid = getpid();
    write_log(pid, client_buffer);
}

// void* handle_connection(int pid, void* ptr_client_socket, int k) {
void* handle_connection(void* ptr_client_socket) {
    int client_socket = *((int*) ptr_client_socket);
    free(ptr_client_socket);

    sem_wait(&mutex);

    char filename[] = "resultados.txt";
    FILE* ptr;
    ptr = fopen(filename, "a");

    if (ptr == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // char pid_str[BUFFER_SIZE];
    // snprintf(pid_str, BUFFER_SIZE, "%d", pid); // copy int pid to char pid_str

    time_t current_time = time(NULL);
    char * current_time_str = ctime(&current_time);
    current_time_str[strlen(current_time_str)-1] = '\0';
    printf("Current Time : %s\n", current_time_str);

    // fprintf(ptr, "Processo: %s\n", pid_str);
    fprintf(ptr, "Hora: %.*s; ", 8, current_time_str + strlen(current_time_str) - 13);
    
    sem_post(&mutex);

    fclose(ptr);
    sleep(k);
    --ongoing_connections;
    return NULL;
}

void start_connection(int client_socket) {
    ++ongoing_connections;
    pthread_t t;
    int *ptr_client = malloc(sizeof(int));
    *ptr_client = client_socket;
    pthread_create(&t, NULL, handle_connection, ptr_client);
}

void next_connection() {
    client_socket = dequeue(&socket_queue);

    if (client_socket != -1) {
        start_connection(client_socket);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Número incorreto de parâmetros passados. Encerrando programa.\n");
        return 1;
    }
    else {
        // n -> Número de processos que querem entrar na região crítica
        // r -> Quantidade de repetições de acesso à região crítica
        // k -> Segundos em que processo deve permanecer na região crítica
        n = atoi(argv[1]);
        r = atoi(argv[2]);
        k = atoi(argv[3]);

        // Inicializando semáforo
        sem_init(&mutex, 0, 1);

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
        if(listen(server_socket, max_connections) < 0){
            printf("Error ao aguardar conexão.\n");
            return -1;
        }
        printf("Aguardando conexão.\n");

        while(1) {
            client_size = sizeof(client_address);
            client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_size);
            printf("Conexão estabelecida com sucesso.\n");

            handle_message(client_socket);
            if (ongoing_connections > max_connections) {
                enqueue(&socket_queue, client_socket);
            } else {
                start_connection(client_socket);
            }
        }
        return 0;
    }
}