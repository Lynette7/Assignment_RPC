//Iterative-connectionless program 
//use SOCK_DGRAM to transmit data in form of packets
//it transfers the data packets from client to server without any connect

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 8889

int main() {
    char buffer[BUFFER_SIZE];

    // Socket creation
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        printf("ERROR IN SOCKET CREATION.\n");
        exit(EXIT_FAILURE);
    }

    // Server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Input registration number and name
    char registrationNum[20];
    char name[50];
    printf("Enter Registration Number: ");
    fgets(registrationNum, sizeof(registrationNum), stdin);
    printf("Enter Name: ");
    fgets(name, sizeof(name), stdin);

    // Combine registration number and name into a single message
    sprintf(buffer, "%s %s", registrationNum, name);

    // Send message to the server
    if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr,
               sizeof(serverAddr)) == -1) {
        perror("Sendto failed");
        exit(EXIT_FAILURE);
    }

    // Receive response from the server
    memset(buffer, 0, BUFFER_SIZE);
    socklen_t serverAddrLen = sizeof(serverAddr);
    ssize_t receivedBytes = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0,
                                     (struct sockaddr *)&serverAddr, &serverAddrLen);

    if (receivedBytes == -1) {
        perror("Recvfrom failed");
        exit(EXIT_FAILURE);
    }

    printf("Server response: %s\n", buffer);

    // Close the socket
    //close(clientSocket);

    return 0;
}
