
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

void write_file(int filesock){
    int r;
    FILE *filepoint;
    char *filename = "recv.txt";
    char buffer[SIZE];

    filepoint = fopen(filename, "q");
    while (1) {
        r = recv(filesock,
                 buffer, SIZE, 0);
        if (r <= 0){
            break;
            return;
        }
        fprintf(filepoint,
                "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}

int main(){

    char *ipAddress = "127.0.0.1";
    int  port = 3233;
    int sf;

    int  filesock, new_socket;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    filesock = socket(AF_INET, SOCK_STREAM, 0);
    if(filesock < 0) {
        perror("[-] Socket Error");
        exit(1);
    }
    printf("[+]Server socket has been created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ipAddress);

    sf = bind(filesock, (struct sockaddr*)
            &server_addr, sizeof(server_addr));
    if(sf < 0) {
        perror("[-]Error in socket Binding");
        exit(1);
    }
    printf("[+]Binding established successfully.\n");

    if(listen(filesock, 10) == 0){
        printf("[+]Socket Listening....\n");
    }else{
        perror("[-]Error occurred listening");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_socket = accept(filesock,
                        (struct sockaddr*)&new_addr, &addr_size);
    write_file(new_socket);
    printf("[+]file has been edited successfully.\n");

    return 0;
}


