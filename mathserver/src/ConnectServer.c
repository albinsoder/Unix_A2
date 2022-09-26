#include "../include/ConnectServer.h"

// #include "../src/matrix_inverse.c"

void initialize(int port){

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
    // Error handling if socket id is not valid
    if (sockfd < 0) {
        printf("Error in connection.\n");
        exit(1);
    }
 
    printf("Server Socket is created.\n");
 
    // Initializing address structure with NULL
    memset(&serverAddr, '\0',sizeof(serverAddr));
 
    // Assign port number and IP address
    // to the socket created
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
 
    // 127.0.0.1 is a loopback address
    serverAddr.sin_addr.s_addr= inet_addr("127.0.0.1");
 
    // Binding the socket id with
    // the socket structure
    ret = bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
 
    // Error handling
    if (ret < 0) {
        printf("Error in binding.\n");
        exit(1);
    }
    // Listening for connections (upto 10)
    cnt =0;
    
};

void serverInterface(){
    
    int connected = 1;
    if (listen(sockfd, 100) == 0) {
        printf("Listening for clients...\n\n");
    }
    while (connected) {
        // Accept clients and
        // store their information in cliAddr
        // printf("Hello");
        clientSocket = accept(
            sockfd, (struct sockaddr*)&cliAddr,
            &addr_size);
 
        // Error handling
        if (clientSocket < 0) {
            exit(1);
        }
 
        // Print number of clients
        // connected till now
        cnt++;
        printf("Connected with client: %d  \n",cnt);
 
        // Creates a child process
        if ((childpid = fork()) == 0) {
            // Closing the server socket id
            close(sockfd);
            send(clientSocket, "hi client",
                strlen("hi client"), 0);
            while(connected){
                int res = recv(clientSocket, buffer, 1024, 0);
                if(res == 0){
                    close(clientSocket);
                    kill(getpid(), SIGKILL);
                    break;
                }
                printf("Client: %d commanded:", cnt);
                strncpy(copyBuffer, buffer, sizeof(buffer));
                commandRes = readBuffer(copyBuffer);
                if (commandRes == -1)
                {
                    send(clientSocket, "Faulty input, please input new command!",40, 0);
                }
                
                puts(buffer);
                bzero(buffer, sizeof(buffer));
                bzero(copyBuffer, sizeof(buffer));
                // bzero(buffer, sizeof(buffer));

                send(clientSocket, "Sending solution: ",strlen("Sending solution:"), 0);
            }
        }
    }
 
    // Close the client socket id
    close(clientSocket);
}

int readBuffer(char** copyBuffer){

    firstArg = strtok(copyBuffer, " ");
    strncpy(buff, copyBuffer, sizeof(copyBuffer));
    char* kmeans = "kmeans";
    char* matinv = "matinvpar";
    int args = 0;

    if(strcmp(firstArg, kmeans) == 0){
        printf("kmeans");
        // Count number of args
        args = countArg(copyBuffer);
        //call kmeans function
    }
    else if(strcmp(firstArg, matinv) == 0){
        printf("matinv");
        args = countArg(copyBuffer);
        // startMat(args, buff);
    }
    else {
        printf("Faulty input");
        return -1;
    }


    return 0;
}

int countArg(char** copyBuffer){
    int count=0;
    for(int i=0; i<sizeof(&copyBuffer); i++){
        if(copyBuffer[i] == "-"){
            count++;
        }
    }
    return count++;
}