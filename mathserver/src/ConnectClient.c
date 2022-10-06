#include "../include/ConnectClient.h"

void initialize(int port, char* ip){

    clientSocket = socket(AF_INET,
                          SOCK_STREAM, 0);
 
    if (clientSocket < 0) {
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Client Socket is created.\n");
 
    // Initializing socket structure with NULL
    memset(&cliAddr, '\0', sizeof(cliAddr));
 
    // Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));
 
    // Assigning port number and IP address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
 
    // 127.0.0.1 is Loopback IP
    serverAddr.sin_addr.s_addr
        = inet_addr(ip);

};

void connectToServer(char clientNr){
    ret = connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
 
    if (ret < 0) {
        printf("Error in connection.\n");
        exit(1);
    }
 
    printf("Connected to Server.\n");

}

void clientInterface(){
    int connected =1;
    if (recv(clientSocket, buffer, 1024, 0)< 0) {
        printf("Error in receiving data.\n");
    }
    else {
        printf("Server: %s\n", buffer);
        bzero(buffer, sizeof(buffer));
    }
    newMsg = NULL;
    while (connected == 1) {

        getline(&newMsg, &len, stdin);
        send(clientSocket, newMsg,strlen(newMsg), 0);
        fflush(stdin);
        // recv() receives the message
        // from server and stores in buffer
        FILE* fp;
        int size = 4096;
        char* data = malloc(size);

        fp = fopen("../../computed_results/matrix_result.txt", "w");
        int n;

        while(1){
            n = recv(clientSocket, data, size, 0);
            printf("%s \n", data);
            printf("N: %d \n", n);
            if (n == 1)
            {   
                printf("KLARA NU DED\n");
                break;
            }
            fprintf(fp, "%s", data);
            bzero(data, size);
        }
        fclose(fp);
        free(data);
        printf("DONE \n");
        // puts(buffer);
        // bzero(buffer, sizeof(buffer));
        // Printing the message on screen
    }
    close(clientSocket);

 }
