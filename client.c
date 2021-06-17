/* C header files */

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

/* Socket API headers */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE 1024

void send_file(FILE *filepoint, int filesock){
    int n;
    char data[SIZE] = {0};

    while(fgets(data, SIZE, filepoint) != NULL) {
        if (send(filesock, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main(){

    char *ipAddress  = "127.0.0.1";
    int  port = 3233;
    int sf;

    int filesock;
    struct sockaddr_in server_addr;
    FILE *filepoint;
    char *filename = "CMPE431.txt";

    filesock = socket(AF_INET, SOCK_STREAM, 0);
    if(filesock < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ipAddress);

    sf= connect(filesock, (struct sockaddr*)
            &server_addr, sizeof(server_addr));
    if(sf == -1) {
        perror("[-] socket connection error");
        exit(1);
    }
    printf("[+]Successfully Connected to Server.\n");

    filepoint = fopen(filename, "r");
    if (filepoint == NULL) {
        perror("[-]Error in reading file.");
        exit(1);
    }

    send_file(filepoint, filesock);
    printf("[+]File data sent successfully.\n");

    printf("[+]Closing the connection.\n");
    close(filesock);

    return 0;
}