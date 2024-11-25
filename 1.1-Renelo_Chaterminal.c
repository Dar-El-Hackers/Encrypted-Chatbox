#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>


#define PORT_NUM 12345
#define MAX_MESS 500
struct termios oldt;

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

    tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
}

void 
sendMessage(int fd_socket) {
    tcflush(STDIN_FILENO,TCIFLUSH);
    char sendMessage[MAX_MESS];
    fgets(sendMessage,MAX_MESS,stdin);
    write(fd_socket,sendMessage,MAX_MESS);
}

void 
recvMessage(int fd_socket) {
    char recvMessage[MAX_MESS];
    read(fd_socket,recvMessage,MAX_MESS);
    fputs(recvMessage,stdout);
    
}

int 
main (int argc, char **argv) {
    //The RENELO chaterminal accept only one argument which is the IP address of the second party and should be launched as follows: "RENELO <IP ADDRESS>"
    if (argc!=2)
        printf("Error arguments: RENELO <IP ADDRESS>");

    //Step A: Trying to connect to the second party. ("ALLO Anyone on the line ?") Now when launching RENELO there are 2 possibilities: the second party will be already live and listening so we should be able to directly connect OR the second party is not live so we will continue with Step B and start a listen socket
    int fd_connect,fd_listen,fd_connection,stepA;
    fd_connect=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in connect_addr;
    connect_addr.sin_port=htons(PORT_NUM); //setting the port number of the connect address to the port number defined above
    connect_addr.sin_family=AF_INET; //setting the connect address to be IPv4
    inet_pton(AF_INET,argv[1],&connect_addr.sin_addr);

    printf("Trying to connect to Majnoun\n");
    stepA=connect(fd_connect,(struct sockaddr *) &connect_addr,sizeof(connect_addr));
    

    //Step B: In case step A fails we will start a listen socket 
    if (stepA!=0) {
        printf("Majnoun appears to be offline, starting mohiohio listening\n");
        struct sockaddr_in listen_addr,connection_addr;
        socklen_t clilen;
        fd_listen=socket(AF_INET,SOCK_STREAM,0);
        memset(&listen_addr,0,sizeof(listen_addr)); //this will set all fields in listen_addr to null values
        listen_addr.sin_port=htons(PORT_NUM); //setting the port number of the listen address to the port number defined above
        listen_addr.sin_family=AF_INET; //setting the listen address to be IPv4
        listen_addr.sin_addr.s_addr=htonl(INADDR_ANY); //setting the IP of listen address to the IP address of all interfaces of the system.
        if (bind(fd_listen,(struct sockaddr *) &listen_addr,sizeof(listen_addr))!=0)
            printf("Was not able to bind\n");
        if (listen(fd_listen,5)!=0)
            printf("Was not able to listen\n");
        
        clilen=sizeof(connection_addr);
        if ((fd_connection=accept(fd_listen,(struct sockaddr*) &connection_addr,&clilen))>=0) {
            printf("Majnoun connected to the line\n");
            
        }
    }

    if (stepA==0) {
        printf("Connected to Majnoun's Line\n");
    }

    /*Messaging system: The messaging system will be turned based, the party who will be acting as the server(opened the listening socket first) will be able to send a message first. It is important to know also that a maximum messaging time is set to 5 minutes. In case 5 minutes elapsed and the party did not send a message, then the turn will be reversed.
    The following will be implemented with threads and a mutex lock.
    */

    tcgetattr(STDIN_FILENO,&oldt);

    for (;;) {
        if (stepA!=0) {
            printf("[ME]: ");
            sendMessage(fd_connection);
            blockTyping();
            printf("[MAJNOUN]: ");
            recvMessage(fd_connection);
            restoreTyping();
        }
        else {
            blockTyping();
            printf("[MAJNOUN]: ");
            recvMessage(fd_connect);
            printf("[ME]: ");
            restoreTyping();
            sendMessage(fd_connect);
        }
    }

}
