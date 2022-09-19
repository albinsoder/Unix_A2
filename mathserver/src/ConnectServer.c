#include "../include/ConnectServer.h"

void initialize(int port){

    bzero(&servaddr, sizeof(servaddr));

    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);        
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    servaddr.sin_family = AF_INET; 
   
    // bind server address to socket descriptor
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

};

void serverInterface(){
    len = sizeof(cliaddr);
    connected = 1;
    while(connected == 1){
        int n = recvfrom(listenfd, buffer, sizeof(buffer),
                0, (struct sockaddr*)&cliaddr,&len); //receive message from client

        fprintf(stderr, "%s", buffer);
    }
}
