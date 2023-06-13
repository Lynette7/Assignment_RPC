#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include<arpa/inet.h>

#define BUFFER_SIZE 1024

// Function to perform the desired operation
double calculate(double num1, double num2, char operation) {
    double result;
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

void handle_client(int client_socket, struct sockaddr_in* client_addr) {
    char buffer[BUFFER_SIZE];
    char client_ip[INET_ADDRSTRLEN];

    // Get client IP address
    inet_ntop(AF_INET, &(client_addr->sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("Client connected from %s\n", client_ip);

    // Receive data from client
    ssize_t recvlen = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (recvlen < 0) {
        printf("ERROR in receiving data from client %s\n", client_ip);
        return;
    }

    // Parse the received data
    double num1, num2;
    char operation;
    sscanf(buffer, "%lf %lf %c", &num1, &num2, &operation);

    // Perform the operation
    double result = calculate(num1, num2, operation);

    // Convert the result to a string
    char result_str[100];
    sprintf(result_str, "Result: %.2lf\n", result);

    // Send the result back to the client
    ssize_t sendlen = send(client_socket, result_str, strlen(result_str), 0);
    if (sendlen < 0) {
        printf("ERROR in sending data to client %s\n", client_ip);
        return;
    }

    // Print the client's IP address and data on the server side
    printf("Client %s: Num1 = %.2lf, Num2 = %.2lf, Operation = %c, Result = %.2lf\n", client_ip, num1, num2, operation, result);
}

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
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("ERROR in binding.\n");
        return 1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        printf("ERROR in listening.\n");
        return 1;
    }

    printf("Calculator server started. Waiting for clients...\n");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        // Accept a new client connection
        int client_socket = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            printf("ERROR in accepting a client connection.\n");
            continue;
        }

        // Create a child process to handle the client
        pid_t pid = fork();
        if (pid < 0) {
            printf("ERROR in creating a child process.\n");
            close(client_socket);
            continue;
        }

        if (pid == 0) {
            // Child process
            close(sockfd);  // Close the listening socket in the child process
            handle_client(client_socket, &client_addr);
            close(client_socket);
            exit(0);
        } else {
            // Parent process
            close(client_socket);  // Close the client socket in the parent process
        }
    }

    // Close the socket
    //close(sockfd);

    return 0;
}
