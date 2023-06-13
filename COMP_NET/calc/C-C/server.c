#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#define BUFFER_SIZE 1024

// Function to perform the desired operation
int calculate(int num1, int num2, char operation) {
    int result;
    switch (operation) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0)
                result = num1 / num2;
            else {
                printf("Error: Division by zero\n");
                result = 0;
            }
            break;
        default:
            printf("Error: Invalid operation\n");
            result = 0;
    }
    return result;
}

int main() {
    char buffer[BUFFER_SIZE];

    // Socket creation
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("ERROR in socket creation.\n");
        return 1;
    }

    // Specifying the server address
    struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9000);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("ERROR in binding.\n");
        return 1;
    }

    printf("Calculator server started. Waiting for clients...\n");

    while (1) {
        socklen_t clilen = sizeof(cliaddr);
        memset(buffer, 0, BUFFER_SIZE);

        // Receive data from client
        ssize_t recvlen = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&cliaddr, &clilen);
        if (recvlen < 0) {
            printf("ERROR in receiving data.\n");
            continue;
        }

        // Parse the received data
        int num1, num2;
        char operation;
        sscanf(buffer, "%d %d %c", &num1, &num2, &operation);

        // Perform the operation
        int result = calculate(num1, num2, operation);

        // Convert the result to a string
        char result_str[10];
        sprintf(result_str, "%d", result);

        // Send the result back to the client
        ssize_t sendlen = sendto(sockfd, result_str, strlen(result_str), 0, (struct sockaddr*)&cliaddr, clilen);
        if (sendlen < 0) {
            printf("ERROR in sending data.\n");
            continue;
        }
    }

    // Close the socket
    //close(sockfd);

    return 0;
}
