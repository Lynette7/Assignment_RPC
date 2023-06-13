#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#define BUFFER_SIZE 1024

void get_web_page(const char* server, const char* path) {
    struct hostent* host = gethostbyname(server);
    if (host == NULL) {
        printf("ERROR: Failed to resolve host.\n");
        return;
    }
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);
    memcpy(&server_address.sin_addr, host->h_addr_list[0], host->h_length);

    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR: Failed to create socket.\n");
        return;
    }
    
    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        printf("ERROR: Failed to connect to the server.\n");
        return;
    }
    
    char request[BUFFER_SIZE];
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, server);
    
    if (send(sockfd, request, strlen(request), 0) < 0) {
        printf("ERROR: Failed to send request to the server.\n");
        return;
    }
    
    char response[BUFFER_SIZE];
    memset(response, 0, BUFFER_SIZE);
    
    while (recv(sockfd, response, BUFFER_SIZE - 1, 0) > 0) {
        printf("%s", response);
        memset(response, 0, BUFFER_SIZE);
    }
    
    //close(sockfd);
}

int main() {
    char server[100];
    char path[100];
    
    printf("Enter the web server: ");
    fgets(server, sizeof(server), stdin);
    server[strcspn(server, "\r\n")] = '\0';
    
    printf("Enter the path: ");
    fgets(path, sizeof(path), stdin);
    path[strcspn(path, "\r\n")] = '\0';
    
    printf("Retrieving web page from server: %s\n", server);
    
    get_web_page(server, path);
    
    return 0;
}
