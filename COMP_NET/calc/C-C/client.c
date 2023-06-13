#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];

    // Socket creation
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("ERROR in socket creation.\n");
        return 1;
    }

    // Server address
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9000);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    while (1) {
        // User input
        int num1, num2;
        char operation;

        printf("Enter the first number: ");
        scanf("%d", &num1);
        getchar(); // consume the newline character

        printf("Enter the second number: ");
        scanf("%d", &num2);
        getchar(); // consume the newline character

        printf("Enter the operation (+, -, *, /): ");
        scanf("%c", &operation);
        getchar(); // consume the newline character

        // Send data to server
        sprintf(buffer, "%d %d %c", num1, num2, operation);
        ssize_t sendlen = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
        if (sendlen < 0) {
            printf("ERROR in sending data.\n");
            continue;
        }

        // Receive result from server
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t recvlen = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (recvlen < 0) {
            printf("ERROR in receiving data.\n");
            continue;
        }

        // Print the result
        printf("Result: %s\n", buffer);
    }

    // Close the socket
    //close(sockfd);

    return 0;
}
