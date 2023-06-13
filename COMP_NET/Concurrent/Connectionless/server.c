#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8888
#define FILE_NAME "registration.txt"

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[MAX_BUFFER_SIZE];
    socklen_t clientAddrLen;

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
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

    printf("Server started. Listening on port %d...\n", SERVER_PORT);

    FILE *file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Receive data from client
        memset(buffer, 0, MAX_BUFFER_SIZE);
        clientAddrLen = sizeof(clientAddr);
        ssize_t receivedBytes = recvfrom(serverSocket, buffer, MAX_BUFFER_SIZE, 0,
                                         (struct sockaddr *)&clientAddr, &clientAddrLen);

        if (receivedBytes == -1) {
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }

        printf("Received data from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        printf("Data: %s\n", buffer);

        // Fork a new process to handle the client
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            // Save registration number and name in the file
            fprintf(file, "%s\n", buffer);
            fflush(file);

            // Send acknowledgment to client
            const char *ack = "Data received";
            if (sendto(serverSocket, ack, strlen(ack), 0, (struct sockaddr *)&clientAddr, clientAddrLen) == -1) {
                perror("Sendto failed");
                exit(EXIT_FAILURE);
            }

            // Exit the child process
            exit(EXIT_SUCCESS);
        }
    }

    // Close the file
    fclose(file);

    // Close the server socket
    close(serverSocket);

    return 0;
}
