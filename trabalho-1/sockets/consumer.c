#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int is_prime(int n) {
    int i;
    for (i = 2; i <= n / 2; ++i) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main(void)
{
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");
    
    // Listen for clients:
    if(listen(socket_desc, 1) < 0){
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");
    
    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    
    if (client_sock < 0){
        printf("Can't accept\n");
        return -1;
    }
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    bool consuming = true;
    int send_rv;
    int recv_rv;
    printf("P0\n");
    while (consuming) {
        printf("P1\n");
        recv_rv = recv(client_sock, client_message, sizeof(client_message), 0);
        if (recv_rv == -1) {
            printf("Client message falhou.\n");
            return -1;
        }
        int x = atoi(client_message);
        printf("x = %d\n", x);
        if (x == 0) {
            consuming = false;
        } else {
            if (is_prime(x)) {
                // Respond to client:
                strcpy(server_message, "O número enviado é primo.\n");
                send_rv = send(socket_desc, server_message, strlen(server_message)+1, 0);
                if (send_rv == -1) {
                    printf("Server message falhou.\n");
                }
                sleep(1);
            } else {
                // Respond to client:
                strcpy(server_message, "O número enviado não é primo.\n");
                send_rv = send(socket_desc, server_message, strlen(server_message)+1, 0);
                if (send_rv == -1) {
                    printf("Server message falhou.\n");
                }
                sleep(1);
            }
            sleep(1);
        }
    }
    
    // Closing the socket:
    close(client_sock);
    close(socket_desc);
    
    return 0;
}
