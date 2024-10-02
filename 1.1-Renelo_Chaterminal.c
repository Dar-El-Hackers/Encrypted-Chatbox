#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT_NUM 19611

int 
main (int argc, char **argv) {
    //The RENELO chaterminal accept only one argument which is the IP address of the second party and should be launched as follows: "RENELO <IP ADDRESS>"
    if (argc!=2)
        printf("Error arguments: RENELO <IP ADDRESS>");

    //Step A: Trying to connect to the second party. ("ALLO Anyone on the line ?") Now when launching RENELO there are 2 possibilities: the second party will be already live and listening so we should be able to directly connect OR the second party is not live so we will continue with Step B and start a listen socket
    int fd_connect,stepA;
    struct sockaddr_in connect_addr;
    connect_addr.sin_port=htons(PORT_NUM); //setting the port number of the connect address to the port number defined above
    connect_addr.sin_family=AF_INET; //setting the connect address to be IPv4
    inet_pton(AF_INET,argv[1],&connect_addr.sin_addr);

    printf("Reached here\n");
    stepA=connect(fd_connect,(struct sockaddr *) &connect_addr,sizeof(connect_addr));
    printf("Reached here\n");

    //Step B: In case step A fails we will start a listen socket 
    if (stepA!=0) {
        printf("Reached here\n");
        int fd_listen,fd_connection;
        struct sockaddr_in listen_addr,connection_addr;
        socklen_t clilen;
        listen_addr.sin_port=htons(PORT_NUM); //setting the port number of the listen address to the port number defined above
        listen_addr.sin_family=AF_INET; //setting the listen address to be IPv4
        listen_addr.sin_addr.s_addr=htonl(INADDR_ANY); //setting the IP of listen address to the IP address of all interfaces of the system.
        memset(&listen_addr,0,sizeof(listen_addr)); //this will set all fields in listen_addr to null values
        bind(fd_listen,(struct sockaddr *) &listen_addr,sizeof(listen_addr));
        listen(fd_listen,0);
        clilen=sizeof(connection_addr);
        if ((fd_connection=accept(fd_listen,(struct sockaddr*) &connection_addr,&clilen))==0)
            printf("Majnoun connected to the line");
        printf("Reached here\n");
    }

    if (stepA==0)
        printf("Connected to Majnoun's Line");
    
}