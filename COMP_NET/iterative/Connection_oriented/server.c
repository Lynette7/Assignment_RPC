#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8428
#define FILE_NAME "registration.txt"

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer1[MAX_BUFFER_SIZE];
    char buffer2[MAX_BUFFER_SIZE];
    socklen_t clientAddrLen;
    int i;
    i = 1;

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset((char *)&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind socket to server address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Listening on port %d...\n", SERVER_PORT);

    FILE *file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Accept client connection
        clientAddrLen = sizeof(clientAddr);
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) == -1) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Accepted a new client connection from %s:%d\n", inet_ntoa(clientAddr.sin_addr),
               ntohs(clientAddr.sin_port));

        // Send welcome message to client
        const char *welcomeMsg = "Welcome! Please enter your registration number and name.";
        if (send(clientSocket, welcomeMsg, strlen(welcomeMsg), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        // Receive registration number from client
        memset(buffer1, 0, MAX_BUFFER_SIZE);
        ssize_t receivedBytes = recv(clientSocket, buffer1, MAX_BUFFER_SIZE, 0);

        if (receivedBytes == -1) {
            perror("Recv failed");
            exit(EXIT_FAILURE);
        }

        printf("Received registration number from %s:%d - %s\n", inet_ntoa(clientAddr.sin_addr),
               ntohs(clientAddr.sin_port), buffer1);

        // Send acknowledgment to client
        const char *ack = "Registration number received";
        if (send(clientSocket, ack, strlen(ack), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        // Receive name from client
        memset(buffer2, 0, MAX_BUFFER_SIZE);
        receivedBytes = recv(clientSocket, buffer2, MAX_BUFFER_SIZE, 0);

        if (receivedBytes == -1) {
            perror("Recv failed");
            exit(EXIT_FAILURE);
        }

        printf("Received name from (%s:%d) - %s\n", inet_ntoa(clientAddr.sin_addr),
               ntohs(clientAddr.sin_port), buffer2);

        // Save registration number and name in the file
        fprintf(file, "%d\t%s\t\t%s\n",i,buffer1, buffer2);
        fflush(file);
        i++;
        // Send acknowledgment to client
        if (send(clientSocket, ack, strlen(ack), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        // Close the client socket
        close(clientSocket);
    }

    // Close the file
    fclose(file);

    // Close the server socket
    //close(serverSocket);

    return 0;
}
