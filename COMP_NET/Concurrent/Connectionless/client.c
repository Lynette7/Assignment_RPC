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
#define SERVER_PORT 8888

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
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

    while (1) {
        // Prompt for registration number
        printf("Enter registration number (or 'q' to quit): ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        // Check if the user wants to quit
        if (strcmp(buffer, "q") == 0) {
            break;
        }

        // Send registration number to server
        if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
            perror("Sendto failed");
            exit(EXIT_FAILURE);
        }

        // Receive acknowledgment from server
        memset(buffer, 0, MAX_BUFFER_SIZE);
        socklen_t serverAddrLen = sizeof(serverAddr);
        ssize_t receivedBytes = recvfrom(clientSocket, buffer, MAX_BUFFER_SIZE, 0,
                                         (struct sockaddr *)&serverAddr, &serverAddrLen);

        if (receivedBytes == -1) {
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }

        printf("Server: %s\n", buffer);

        // Prompt for name
        printf("Enter name: ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        // Send name to server
        if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
            perror("Sendto failed");
            exit(EXIT_FAILURE);
        }

        // Receive acknowledgment from server
        memset(buffer, 0, MAX_BUFFER_SIZE);
        receivedBytes = recvfrom(clientSocket, buffer, MAX_BUFFER_SIZE, 0,
                                 (struct sockaddr *)&serverAddr, &serverAddrLen);

        if (receivedBytes == -1) {
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }

        printf("Server: %s\n", buffer);
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
