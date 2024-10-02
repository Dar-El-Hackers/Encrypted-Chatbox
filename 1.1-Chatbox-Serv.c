#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT_NUM 15500 //Should be bigger than 5000

int
main (int argc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid; //pid_t is a defined type used to store processes ID, it's an unsigned integer.
    socklen_t clilen; //socklen_t is a defined type used to store the length of the socket address, it's an unsigned integer.
    struct sockaddr_in cliaddr, servaddr; 
    
    /*
    struct sockaddr_in {
        sa_family_t sin_family; //it's an unsigned integer to define the protocol family. AF_INET as sockaddr_in is used for IPv4 addresses
        in_port_t sin port; //it's an uint16_t type so only 16 bits
        struct in_addr sin_addr;
    };

    struct in_addr {
        in_addr_t s_addr;
    };
    */

   listenfd=socket(AF_INET, SOCK_STREAM, 0); //opened a socket which is AF_INET (Layer 3 to be IPv4) ; SOCK_STREAM (Layer 4: TCP) ; the final argument is for protocol in the case of IP its 0

   bzero(&servaddr,sizeof(servaddr));

   servaddr.sin_family=AF_INET; 
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY); //bind to all interfaces of the OS so all IP addresses
   servaddr.sin_port=htons(PORT_NUM); //convert host short unsigned integer to network byte short unsigned integer 
   //Important note: SERV_PORT cannot be used as when communicating together we should know which port to communicate to

   bind(listenfd,(struct sockaddr *) &servaddr,sizeof(servaddr));

   listen(listenfd,0); //I am not sure about setting the backlog to 0, its used to set the number of pending conncetions. In our case the queue should never get full as the program should communicate with only 1 client

   for (;;) {
        clilen=sizeof(cliaddr);
        connfd=accept(listenfd,(struct sockaddr *) &cliaddr,&clilen);
        printf("Client connected\n");
        if ((childpid=fork())==0) { //for every connection established a new process will be created, again in our case this will not be necessary as there is no way that our program connects with more than one client
            close(listenfd); //or otherwise a domino effect may happen
            //here our messaging should start
            exit(0); //exit the process so it doesn't stay open
        }

        close(connfd); //the parent so the main program closes the socket
   }

}