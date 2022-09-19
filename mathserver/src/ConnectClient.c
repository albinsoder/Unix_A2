#include "../include/ConnectClient.h"

void initialize(int port, char* ip){

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);
    servaddr.sin_family = AF_INET;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

};

void connectToServer(char clientNr){
    newMsg = NULL;
    // for (int i = 0; i < message; i++)
    // {
    //     message[i] = " Client1 connected";
    // }
    
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }else {
        // strcat(message, clientNr);
        // char concat[21] = " ,Connected to server";
        // strcat(message, concat);
        connected=1;
        sendto(sockfd, message, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));
    }



}

void clientInterface(){
    while(connected == 1){
        getline(&newMsg, &len, stdin);
        sendto(sockfd, newMsg, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));
        fflush(stdin);
    }

    close(sockfd);
}
