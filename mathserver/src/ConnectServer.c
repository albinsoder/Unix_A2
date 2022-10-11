#include "../include/ConnectServer.h"

void initialize(int port){
    struct stat dirCreator = {0};
    // Create needed directories for input and output data
    if (stat("../../computed_results", &dirCreator) == -1) {
        mkdir("../../computed_results", 0777);
    }
    if (stat("../../computed_results/server_results", &dirCreator) == -1){
        mkdir("../../computed_results/server_results", 0777);
    }
    if (stat("../../computed_results/server_inputs", &dirCreator) == -1){
        mkdir("../../computed_results/server_inputs", 0777);
    }

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
            char cntHolder[4];
            sprintf(cntHolder, "%d", cnt);
            send(clientSocket, cntHolder, strlen(cntHolder), 0);
            while(connected){
                printf("REDO FÖR NÄSTA \n");
                int res = recv(clientSocket, buffer, 1024, 0);
                if(res == 0){
                    close(clientSocket);
                    kill(getpid(), SIGKILL);
                    break;
                }
                printf("Client: %d commanded: ", cnt);
                puts(buffer);
                if(buffer[0] =='k'){
                    int size = 4096;
                    int pIDKmeans = cnt;
                    char path[70];
                    sprintf(path, "../../computed_results/server_inputs/kmeans_client%d_input.txt", cnt);
                    int iteration = recFile(pIDKmeans, size, path);
                    char k[1024];
                    recv(clientSocket, k, 1024, 0);
                    printf("K: %s ", k);
                    printf("DONE \n");
                    commandRes = readBuffer(buffer,k, iteration, path);
                    if (commandRes == -1)
                    {
                        send(clientSocket, "Faulty input, please input new command!",40, 0);

                    }
                    // free(k);
                }
                else{
                    if(buffer[0] == 'm'){
                        commandRes = readBuffer(buffer, 0, 0, 0);
                        if (commandRes == -1)
                        {
                            send(clientSocket, "Faulty input, please input new command!",40, 0);

                        }
                        int pID = cnt; // Clients ID
                        char path[70];
                        sprintf(path,"../../computed_results/server_results/matinv_client%d_sol.txt",pID);
                        int size = 4096;
                        sendFile(pID, size, path);
                        free(buffer);
                    }

                }
                commandRes = 0;
            }
        }
    }
    // Close the client socket id
    close(clientSocket);
}

int readBuffer(char* buff, char* k, int N, char* path){
    if(buff[0] == 'k'){ // Kmeans is to be run
        printf("kmeans \n");
        start_kmeans(k, N, path, cnt);
        return 0;
    }
    else if(buff[0] == 'm'){ // Matinv is to be run
        char** tmpBuff = readMessage(buff);
        start_mat(countArg, tmpBuff, cnt);
        return 0;
    }
    else { // Faulty command provided
        return -1;
    }
    return 0;
}

void sendFile(int pID, int size, char* path){
    char* data = malloc(size);
    FILE* f;
    f = fopen(path, "r");
    while(fgets(data, size, f) != NULL){
        printf("%s \n", data);
        if(send(clientSocket, data, size, 0) == -1){
            perror("Sending data failed");
            exit(1);
        }
        bzero(data, size);
    }
    fclose(f);
    free(data);
    send(clientSocket, "", 1, 0); // Data transmission completed
    if (remove(path) == 0){ // Remove file after sending the data to the client
        printf("Solution file deleted successfully!\n");
    } else {
        printf("Failed to delete solution file!\n");
    }
}

int recFile(int pID, int size, char* path){
    char* data = malloc(size);
    FILE* f;
    f = fopen(path, "w");
    int n;
    int iteration =0;
    while(1){
        n = recv(clientSocket, data, size, 0);
        iteration++;
        // printf("I: %d \n", iteration);
        // fflush(stdout);
        
        if (n == 1)
        {   
            printf("KLARA NU DED\n");
            iteration--;
            break;
        }
        fprintf(f, "%s", data);
        bzero(data, size);
    }
    fclose(f);
    free(data);
    return iteration;


}

char** readMessage(char* buff){
    char** tmpBuff = (char**)malloc(1024);
    char* newBuff = malloc(1024);
    int j,len;
    j=0;
    len=0;
    countArg=0;
    for (int i=0; i<1024; i++)          
    {
        newBuff[j] = buff[i];
        j++;
        if(buff[i] == '\0'){
            tmpBuff[len] = (char*)malloc(strlen(newBuff)+1);
            strcpy(tmpBuff[len], newBuff);
            len++;
            free(newBuff);
            break;
        }
        if (buff[i] == ' '){
            countArg++;
            tmpBuff[len] = (char*)malloc(strlen(newBuff)+1);
            newBuff[j] = '\0';
            strcpy(tmpBuff[len], newBuff);
            len++;
            j=0;
            free(newBuff);
            newBuff = malloc(1024);
        }
            
    }
    // free(newBuff);
    return tmpBuff;
    // free(tmpBuff);
}