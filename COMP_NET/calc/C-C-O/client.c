#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];

    // Socket creation
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR in socket creation.\n");
        return 1;
    }

    // Server address
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9021);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to the server IP address

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("ERROR in connecting to the server.\n");
        return 1;
    }

    while (1) {
        // User input
        double num1, num2;
        char operation;

        printf("Enter the first number: ");
        scanf("%lf", &num1);
        getchar(); // consume the newline character

        printf("Enter the second number: ");
        scanf("%lf", &num2);
        getchar(); // consume the newline character

        printf("Enter the operation (+, -, *, /): ");
        scanf("%c", &operation);
        getchar(); // consume the newline character

        // Send data to the server
        sprintf(buffer, "%.2lf %.2lf %c", num1, num2, operation);
        ssize_t sendlen = send(sockfd, buffer, strlen(buffer), 0);
        if (sendlen < 0) {
            printf("ERROR in sending data to the server.\n");
            continue;
        }

        // Receive result from the server
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t recvlen = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (recvlen < 0) {
            printf("ERROR in receiving data from the server.\n");
            continue;
        }

        // Print the result
        printf("Result: %s\n", buffer);
    }

    // Close the socket
    //close(sockfd);

    return 0;
}
