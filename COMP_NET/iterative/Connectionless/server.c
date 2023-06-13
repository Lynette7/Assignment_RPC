//Iterative-connectionless program 
//use SOCK_DGRAM to transmit data in form of packets
//it transfers the data packets from client to server without any connect

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8889
#define FILE_NAME "registration.txt"

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[MAX_BUFFER_SIZE];
    socklen_t clientAddrLen;
    int i = 1;

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

        printf("Received message from (%s:%d) - \n \t %s\n", inet_ntoa(clientAddr.sin_addr),
               ntohs(clientAddr.sin_port), buffer);

        // Parse registration number and name from the received message
        char registrationNum[20];
        char name[50];
        sscanf(buffer, "%s %s", registrationNum, name);

        // Save the registration number and name in the file
        fprintf(file, "%d \t %s \t%s\n",i, registrationNum, name);
        fflush(file);
        i++;
        // Send response to client
        const char *response = "Data saved successfully!";
        if (sendto(serverSocket, response, strlen(response), 0, (struct sockaddr *)&clientAddr,
                   clientAddrLen) == -1) {
            perror("Sendto failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file
    fclose(file);

    // Close the socket
    //close(serverSocket);

    return 0;
}
