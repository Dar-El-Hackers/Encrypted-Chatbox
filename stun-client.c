#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define SERV_PORT 12345
#define MAXLINE 3

/*The idea is that the following program should act as the STUN (Session Traversal utilities for NAT) server where what it does is very simple. It will receive a UDP packet from a client for which it will read the source IP address and the source port and then will reply the following info to the client. In the following way the client will be able to know his public IP Address and the port assigned to him by NAT. 
For future use, once the client will know this info he will be able to transfer this info to the micro-controller*/

int main (int argc,char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    memset(&servaddr,0,sizeof(servaddr));

    //initializing the client socket address
    servaddr.sin_port=htons(SERV_PORT);
    servaddr.sin_family=AF_INET;
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
    char mess[MAXLINE];
    mess[0]='H';
    mess[1]='I';
    socklen_t servlen=sizeof(servaddr);
    for (;;) {
    sleep(2);
    sendto(sockfd,mess,strlen(mess),0,(struct sockaddr*) &servaddr,servlen);
    }
}
