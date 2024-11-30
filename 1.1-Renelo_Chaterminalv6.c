#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define PORT 7080 //Choosing random port above 1023
#define MAX_MESS 500 //Choosing maximum size of message (500 bytes)
struct termios oldt;
char send_message[MAX_MESS], recv_message[MAX_MESS];


void 
blockTyping() {
    //blocking typing in terminal
    struct termios newt;
    newt=oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&newt);
}

void
restoreTyping() {
    //restoring typing in terminal
    tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
}

void 
sendMessage(int fd_socket) {
    tcflush(STDIN_FILENO,TCIFLUSH); //flush any input user might have typed in terminal when typing is blocked
    fgets(send_message,MAX_MESS,stdin);
    write(fd_socket,send_message,MAX_MESS);
}

void 
recvMessage(int fd_socket) {
    read(fd_socket,recv_message,MAX_MESS);
    fputs(recv_message,stdout); 
}



int 
main (int argc, char **argv) {
    if (argc!=2)
        printf("\nError arguments: RENELO <IP ADDRESS> (<SOURCE PORT> <DESTINATION PORT> OPTIONAL)\n");
    

    //Step A: Trying to connect to the second party. When launching RENELO there are 2 possibilities: the second party will be already live and listening so we should be able to directly connect OR the second party is not live, hence we should start a listening socket
    int fd_connect, fd_listen, fd_connection, stepA, opt, sport, dport;
    struct sockaddr_in6 myaddr, connect_addr, listen_addr, connection_addr;
    char party_addr[INET6_ADDRSTRLEN];
    socklen_t clilen;
   
    fd_connect=socket(AF_INET6,SOCK_STREAM,0);
    
    //Setting the following option to avoid delay in case socket bound to this address is in WAIT mode
    opt=1;
    setsockopt(fd_connect,SOL_SOCKET,SO_REUSEPORT, &opt, sizeof(opt));
    
    //Binding the fd_connect socket to an address
    memset(&myaddr,0,sizeof(myaddr));
    myaddr.sin6_family=AF_INET6;
    sport = argc>=3 ? atoi(argv[2]) : PORT; //Giving the user the possibility of setting the source port
    myaddr.sin6_port=htons(sport);
    myaddr.sin6_addr=in6addr_any;
    //inet_pton(AF_INET6,"put your temporary IPv6 addres here",&myaddr.sin6_addr); //I have noticed that on some machines in6addr_any is leading to packets being dropped by the OS, one quick solution is to bind manually the socket to the temporary IPv6 address of the machine.

    if (bind(fd_connect,(struct sockaddr*) &myaddr,sizeof(myaddr))<0) 
        perror("\nWas not able to bind connect socket: ");
    
    //Defining the other party's address
    memset(&connect_addr,0,sizeof(connect_addr));
    connect_addr.sin6_family=AF_INET6; 
    dport = argc==4 ? atoi(argv[3]) : PORT; //Giving the user the possibility of setting the destination port
    connect_addr.sin6_port=htons(dport);
    inet_pton(AF_INET6,argv[1],&connect_addr.sin6_addr);
    printf("\nTrying to connect to party with IPv6 address %s on port %d.\n",argv[1],sport);
    stepA=connect(fd_connect,(struct sockaddr *) &connect_addr,sizeof(connect_addr));

    //In case step A fails we will start a listening socket 
    if (stepA!=0) {
        close(fd_connect);
        perror("\nCould not connect: ");
        printf("\nThe party appears to be offline, starting listening mode for incoming connections.\n");
        
        fd_listen=socket(AF_INET6,SOCK_STREAM,0);

        //Setting the following option to avoid delay in case socket bound to this address is in WAIT mode
        opt=1;
        setsockopt(fd_listen,SOL_SOCKET,SO_REUSEPORT, &opt, sizeof(opt));

        //Binding the fd_listen socket to an address
        memset(&listen_addr,0,sizeof(listen_addr)); 
        listen_addr.sin6_family=AF_INET6; 
        listen_addr.sin6_port=htons(sport); 
        listen_addr.sin6_addr=in6addr_any; 

        if (bind(fd_listen,(struct sockaddr *) &listen_addr,sizeof(listen_addr))!=0) {
            perror("\nWas not able to bind: ");
            return EXIT_FAILURE;
        }
        
        //Starting listening mode
        if (listen(fd_listen,5)!=0) {
            perror("\nWas not able to listen: ");
            return EXIT_FAILURE;
        }
        
        clilen=sizeof(connection_addr);
        if ((fd_connection=accept(fd_listen,(struct sockaddr*) &connection_addr,&clilen))>=0) {
            inet_ntop(AF_INET6,&connection_addr.sin6_addr,party_addr,INET6_ADDRSTRLEN);
            printf("\nA party has connected to the line with IPv6 address %s:. You can disconnect by sending the message 'DISCONNECT'.\n\n", party_addr);
        } else {
            perror ("\nWas not able to accept connect: ");
        }
    }

    if (stepA==0) {
        printf("\nConnected to party's line. You can disconnect by sending the message 'DISCONNECT'.\n\n");
    }

    /*Messaging system: The messaging system will be turn based, the party who will be acting as the server (opened the listening socket first) will be able to send a message first. 
    */

    tcgetattr(STDIN_FILENO,&oldt);

    for (;;) {
        if (stepA!=0) {
            printf("[ME]: ");
            sendMessage(fd_connection);
            if (strcmp(send_message,"DISCONNECT\n") == 0) {
                close(fd_listen);
                close(fd_connection);
                return EXIT_SUCCESS;
            }
            blockTyping();
            printf("[PARTY]: ");
            recvMessage(fd_connection);
            if (strcmp(recv_message,"DISCONNECT\n") == 0) {
                close(fd_listen);
                close(fd_connection);
                return EXIT_SUCCESS;
            }
            restoreTyping();
        }
        else {
            blockTyping();
            printf("[PARTY]: ");
            recvMessage(fd_connect);
            if (strcmp(recv_message,"DISCONNECT\n") == 0) {
                close(fd_connect);
                return EXIT_SUCCESS;
            }
            printf("[ME]: ");
            restoreTyping();
            sendMessage(fd_connect);
            if (strcmp(send_message,"DISCONNECT\n") == 0) {
                close(fd_connect);
                return EXIT_SUCCESS;
            }
        }
 
    }

}
