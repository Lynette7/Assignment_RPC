#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8428

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];
    char buffer1[MAX_BUFFER_SIZE];
    char buffer2[MAX_BUFFER_SIZE];

    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset((char *)&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr)) <= 0) {
        perror("Invalid server address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Receive welcome message from server
    memset(buffer, 0, MAX_BUFFER_SIZE);
    ssize_t receivedBytes = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);

    if (receivedBytes == -1) {
        perror("Recv failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: %s\n", buffer);

    // Prompt for registration number
    printf("Enter registration number: ");
    fgets(buffer1, MAX_BUFFER_SIZE, stdin);
    buffer1[strcspn(buffer1, "\n")] = '\0'; // Remove newline character

    // Send registration number to server
    if (send(clientSocket, buffer1, strlen(buffer1), 0) == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    // Receive acknowledgment from server
    memset(buffer, 0, MAX_BUFFER_SIZE);
    receivedBytes = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);

    if (receivedBytes == -1) {
        perror("Recv failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: %s\n", buffer);

    // Prompt for name
    printf("Enter name: ");
    fgets(buffer2, MAX_BUFFER_SIZE, stdin);
    buffer2[strcspn(buffer2, "\n")] = '\0'; // Remove newline character

    // Send name to server
    if (send(clientSocket, buffer2, strlen(buffer2), 0) == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    // Receive acknowledgment from server
    memset(buffer, 0, MAX_BUFFER_SIZE);
    receivedBytes = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);

    if (receivedBytes == -1) {
        perror("Recv failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: %s\n", buffer);

    // Close the socket
    close(clientSocket);

    return 0;
}
