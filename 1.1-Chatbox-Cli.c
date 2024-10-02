#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT_NUM 15500

int main (int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc!=2)
        printf("Please enter the command as follows: RENELO [IP Address]\n");

    sockfd=socket(AF_INET, SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT_NUM);
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr);

    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    printf("Connected to server. \n");

    exit(0);
}