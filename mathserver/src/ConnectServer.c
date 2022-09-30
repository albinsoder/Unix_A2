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
        buffer = malloc(1024);
        // copyBuffer = malloc(1024);
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
                // strncpy(copyBuffer, buffer, sizeof(buffer));
                commandRes = readBuffer(buffer);
                if (commandRes == -1)
                {
                    send(clientSocket, "Faulty input, please input new command!",40, 0);
                } 
                commandRes = 0;
                puts(buffer);
                free(buffer);
                // free(copyBuffer);
                // bzero(buffer, sizeof(buffer));
                // bzero(copyBuffer, sizeof(buffer));
                // bzero(buffer, sizeof(buffer));

                send(clientSocket, "Sending solution: ",strlen("Sending solution:"), 0);
            }
        }
    }
 
    // Close the client socket id
    close(clientSocket);
}

int readBuffer(char* buff){

    // char* buff = malloc(1024);
    // strncpy(buff, copyBuffer, 1024);
    // firstArg = strtok(copyBuffer, " ");
    char* kmeans = "k";
    char* matinv = "m";
    int args = 0;
    // char* newBuff = buff;
    // char** gBuff = &newBuff;

    printf("%s", firstArg);
    if(buff[0] == 'k'){
        printf("kmeans \n");
        // Count number of args
        args = countArg(buff);
        //call kmeans function
    }
    else if(buff[0] == 'm'){
        printf("matinv \n");
        char** tmpBuff = (char**)malloc(1024);
        char* newBuff = malloc(1024);
        printf("Buff: %s \n", buff);
        args=7;
        int j=0;
        int len=0;
        for (int i=0; i<1024; i++)          
        {
            // printf("Inbuff per gång: %s", &inBuff[i]);
            printf("Buff: %c\n", buff[i]);
            newBuff[j] = buff[i];
            j++;
            if(buff[i] == '\0'){
                tmpBuff[len] = (char*)malloc(strlen(newBuff)+1);
                strcpy(tmpBuff[len], newBuff);
                free(newBuff);
                break;
            }
            if (buff[i] == ' '){
                // puts(tmpBuff);
                tmpBuff[len] = (char*)malloc(strlen(newBuff)+1);
                strcpy(tmpBuff[len], newBuff);
                
                // printf("OUTBUFF: %s", outBuff[len]);
                len++;
                j=0;
                free(newBuff);
                newBuff = malloc(1024);
            }
                
        }
        free(buff);
        
        printf("HALLÅ: %c \n", tmpBuff[0][1]);
        matStart(7, tmpBuff);
        // Init_Default();		/* Init default values	*/
        // Read_Option(7, tmpBuff);	/* Read arguments	*/
        // // tmpBuff = rmWhitespace(buff);
        // Init_Matrix();		/* Init the matrix	*/
        // find_inverse();

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

char** rmWhitespace(char* inBuff)                                         
{
    int len = 0; // Length of current command
    int j=0; // tmpBuff's position at end of a command
    char** outBuff; //2D Array
    char* tmpBuff; //1D Array
    // memset(outBuff, '\0', siz)
    // printf("InBuff: %s", inBuff);
    // printf("strlen:");
    for (int i=0; i<strlen(inBuff); i++)          
    {
        // printf("Inbuff per gång: %s", &inBuff[i]);
        tmpBuff[j] = inBuff[i];
        j++;
        if (inBuff[i] == ' '){
            // puts(tmpBuff);
            // strcpy(outBuff[len], tmpBuff);
            
            // printf("OUTBUFF: %s", outBuff[len]);
            len++;
            j=0;
            bzero(tmpBuff, sizeof(tmpBuff));
        }
            
    }
    printf("Här borde vara n: %c", outBuff[1][2]);
    return outBuff;
}

// void startMat(int argc, char** argv){
//     printf("Här");
//     Init_Default();		/* Init default values	*/
//     Read_Option(argc, argv);	/* Read arguments	*/
//     Init_Matrix();		/* Init the matrix	*/
//     find_inverse();

// }