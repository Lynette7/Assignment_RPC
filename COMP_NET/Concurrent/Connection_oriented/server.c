#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8099
#define FILE_NAME "registration.txt"

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
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

    FILE *file;

    while (1) {
        // Accept a new client connection
        clientAddrLen = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New client connected: %s\n", inet_ntoa(clientAddr.sin_addr));

        // Fork a new process to handle the client
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            close(serverSocket);

            // Open the file in append mode
            file = fopen(FILE_NAME, "a");
            if (file == NULL) {
                perror("File opening failed");
                exit(EXIT_FAILURE);
            }

            char buffer[MAX_BUFFER_SIZE];
            char buffer1[MAX_BUFFER_SIZE];
            char buffer2[MAX_BUFFER_SIZE];
            ssize_t bytesRead;

            // Receive registration number from client
            bytesRead = recv(clientSocket, buffer1, MAX_BUFFER_SIZE, 0);
            if (bytesRead == -1) {
                perror("Recv failed");
                exit(EXIT_FAILURE);
            }

            // Write registration number to file
            fprintf(file,"%d\t%s\t",i, buffer1);
            i++;
            // Receive name from client
            bytesRead = recv(clientSocket, buffer2, MAX_BUFFER_SIZE, 0);
            if (bytesRead == -1) {
                perror("Recv failed");
                exit(EXIT_FAILURE);
            }

            // Write name to file
            fprintf(file, "\t%s\n", buffer2);

            printf("Data written to file.\n");

            // Close the file
            fclose(file);

            // Close the client socket
            close(clientSocket);

            // Exit the child process
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            close(clientSocket);
        }
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
