#include "../include/ConnectServer.h"

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
                commandRes = 0;
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

int readBuffer(char copyBuffer[1024]){

    char buff[1024];
    strncpy(buff, copyBuffer, 1024);
    firstArg = strtok(copyBuffer, " ");
    char* kmeans = "kmeans";
    char* matinv = "matinvpar";
    int args = 0;
    // char* newBuff = buff;
    // char** gBuff = &newBuff;

    if(strcmp(firstArg, kmeans) == 0){
        printf("kmeans \n");
        // Count number of args
        args = countArg(buff);
        //call kmeans function
    }
    else if(strcmp(firstArg, matinv) == 0){
        printf("matinv \n");
        char* tmpBuff;
        // args = countArg(buff);
        args=7;
        // printf("Innan startmat\n");
        Init_Default();		/* Init default values	*/
        Read_Option(buff);	/* Read arguments	*/
        // tmpBuff = rmWhitespace(buff);
        // printf("%s", tmpBuff[0]);
        // printf("ded");
        // char** gBuff = {'-','n'};
        // char disp[2][4] = {
        // {'-', 'n', '-', 'I'},
        // {'a', 'D', 's', 'a'}
        // };
        // [0][0] = "-"
        // Init_Matrix();		/* Init the matrix	*/
        // find_inverse();
        // printf("%s", tmpBuff);
        // printf("Innan startmat ye\n");
        // startMat(args, newBuff);
        return 0;
    }
    else {
        printf("Faulty input");
        return -1;
    }
    return 0;
}

int countArg(char copyBuffer[1024]){
    int count=0;
    for(int j=0; j<1024; j++){
        if(copyBuffer[j] == ' '){
            count++;
        }
    }
    count++;
    return count;
}

// char* rmWhitespace(char* inBuff)                                         
// {
//     int len = 0; // Length of current command
//     int j=0; // tmpBuff's position at end of a command
//     char outBuff[1024]; //2D Array
//     char tmpBuff[1024]; //1D Array
//     // printf("InBuff: %s", inBuff);
//     printf("strlen: %li", strlen(inBuff));
//     for (int i=0; i<strlen(inBuff); i++)          
//     {
//         // printf("Inbuff per gång: %s", &inBuff[i]);
//         tmpBuff[j] = inBuff[i];
//         j++;
//         if (inBuff[i] == ' '){
//             // puts(tmpBuff);
//             outBuff[len] = tmpBuff;
//             printf("OUTBUFF: %s", outBuff[len]);
//             len++;
//             j=0;
//             bzero(tmpBuff, sizeof(tmpBuff));
//         }
            
//     }
//     // printf("%s", outBuff);
//     return outBuff;
// }

// void startMat(int argc, char** argv){
//     printf("Här");
//     Init_Default();		/* Init default values	*/
//     Read_Option(argc, argv);	/* Read arguments	*/
//     Init_Matrix();		/* Init the matrix	*/
//     find_inverse();

// }