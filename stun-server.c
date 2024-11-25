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
#define MAXLINE 500

/*The idea is that the following program should act as the STUN (Session Traversal utilities for NAT) server where what it does is very simple. It will receive a UDP packet from a client for which it will read the source IP address and the source port and then will reply the following info to the client. In the following way the client will be able to know his public IP Address and the port assigned to him by NAT. 
For future use, once the client will know this info he will be able to transfer this info to the micro-controller*/

int main (int argc,char **argv) {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERV_PORT);

    bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    int message_len;
    socklen_t len;
    char mesg[MAXLINE];
    char reply[MAXLINE];

    for (;;) {
        len=sizeof(cliaddr);
        message_len=recvfrom(sockfd,mesg,MAXLINE,0,(struct sockaddr*) &cliaddr,&len);
        char cliIP[MAXLINE];
        inet_ntop(AF_INET,&cliaddr.sin_addr,cliIP,MAXLINE);
        printf("The IP address of the client is: %s\n",cliIP);
        printf("The port number of the client is: %hu\n",ntohs(cliaddr.sin_port));  
    }
}
