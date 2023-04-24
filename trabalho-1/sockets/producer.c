#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    
    // Clean buffers:
    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Unable to create socket\n");
        return -1;
    }
    
    printf("Socket created successfully\n");
    
    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Send connection request to server:
    if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");
    
    int n = 0;
    printf("Informe quantos números devem ser gerados: ");
    scanf("%d", &n);

    int buffer_size = 20;
    char buffer[buffer_size];

    srand(15); // random number generator seed
    int x = 1;
    // char client_message[20];
    int send_rv;

    for (int i=0; i != n; ++i) {
        x = x + 1 + (rand() % 99); // rand() is in {0, 1, ..., RAND_MAX}
        snprintf(client_message, 20, "%d", x); // copy int x to char client_message
        send_rv = send(socket_desc, client_message, strlen(client_message)+1, 0);
        if (send_rv == -1) {
            printf("Client message i={%d} falhou.\n", i);
        }
        sleep(1);
        // Receive the server's response:
        if(recv(socket_desc, server_message, sizeof(server_message), 0) < 0){
            printf("Server response i={%d} falhou.\n", i);
            return -1;
        }
        sleep(1);
    }
    send_rv = send(socket_desc, client_message, strlen(client_message)+1, 0);
    if (send_rv == -1) {
            printf("Client message str(0) falhou.\n");
    }
    sleep(1);

    // Receive the server's response:
    if(recv(socket_desc, server_message, sizeof(server_message), 0) < 0){
        printf("Server response falhou.\n");
        return -1;
    }
    
    // Close the socket:
    close(socket_desc);
    
    return 0;
}
