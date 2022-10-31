#include "../include/ConnectServer.h"

// #define INADDR_ANY 0
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

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // Set up socket file descriptor
 
    // If socket fd creation was unsuccessful
    if (sockfd < 0) {
        printf("Error in connection.\n");
        exit(1);
    }

    printf("Server Socket is created.\n");
 
    // Initialize server address structure as NULL
    memset(&serverAddr, '\0',sizeof(serverAddr));
 
    // Give the port number and IP address to the newly created socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
 
    // Allow any incoming connections, from clients on other hosts and from other networks
    serverAddr.sin_addr.s_addr= INADDR_ANY;
 
    // Bind the socket with the options from above
    ret = bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
 
    // If the binding is unsuccessful
    if (ret < 0) {
        printf("Error in binding.\n");
        exit(1);
    }
    // client will keep track of connected clients by incrementing every time a new connection is established
    client =0;
    
};

void serverInterface(){
    
    int connected = 1; // Connected flag
    if (listen(sockfd, 100) == 0) { // Server currently accepts 100 simultaneously clients
        printf("Listening for clients...\n\n");
    }
    while (connected) {
        // Accept client connection and store the information in cliAddr
        clientSocket = accept(sockfd, (struct sockaddr*)&cliAddr,&addr_size);
 
        if (clientSocket < 0) {
            exit(1);
        }
 
        // Increment client upon new connection and print message
        client++;
        printf("Connected with client: %d  \n",client);
        buffer = malloc(1024);
        // Create new child process for the new connection
        if ((childpid = fork()) == 0) {
            // Closing the server socket id
            close(sockfd);
            char clientHolder[4];
            sprintf(clientHolder, "%d", client);
            send(clientSocket, clientHolder, strlen(clientHolder), 0); // Send what connection ID the client has to the client
            // While connected the server should be able to take new commands
            while(connected){
                // printf("REDO FÖR NÄSTA \n");
                int res = recv(clientSocket, buffer, 1024, 0); // Receive the client command
                if(res == 0){
                    close(clientSocket);
                    kill(getpid(), SIGKILL);
                    break;
                }
                printf("Client: %d commanded: ", client);
                puts(buffer);
                if(buffer[0] =='k'){ // If client commanded kmeans
                    //Variables for handling of size, id, path to input file, k and N
                    int size = 1024;
                    int pIDKmeans = client;
                    char path[70];
                    int iteration; //Iteration = N
                    sprintf(path, "../../computed_results/server_inputs/kmeans_client%d_input.txt", client);

                    //Receive input file from client
                    iteration = recFile(pIDKmeans, size, path);

                    //Read buffer and perform the command from client
                    runAlgorithm(buffer, iteration, path);

                    bzero(path, sizeof(path)); // Clear path array
                    //Path is replaced to be the results folder
                    sprintf(path,"../../computed_results/server_results/kmeans%d_sol.txt",client);
                    //Send back the result file.  
                    sendFile(pIDKmeans,size, path);
                }
                else if(buffer[0] == 'm'){ // If client commanded matinv
                    int size = 4096;
                    int pID = client; // Clients connection ID
                    char path[70];

                    runAlgorithm(buffer, 0, 0); // Read/filter the input and perform the client command

                    sprintf(path,"../../computed_results/server_results/matinv_client%d_sol.txt",pID);
                    sendFile(pID, size, path); // Send the result file to the client
                }
                bzero(buffer, sizeof(buffer)); // Clear buffer
            }
        }
    }
    // Close the client socket id
    close(clientSocket);
}

int runAlgorithm(char* buff, int N, char* path){
    if(buff[0] == 'k'){ // Kmeans is to be run
        char** tmpBuff = malloc(1024);
        char* k = malloc(20);

        tmpBuff = readMessage(buff, tmpBuff); // Filter the client input
        k = getK(tmpBuff, k);

        start_kmeans(k, N, path, client); // Run k-means algorithm
        free(k);

        if (remove(path) == 0){ // Remove input file after reading the data in kmeans
            printf("Input file deleted successfully!\n");
        } else {
            printf("Failed to delete Input file!\n");
        }

        free(tmpBuff); // Free tmpBuff
        return 0; // Done
    }
    else if(buff[0] == 'm'){ // Matinv is to be run
        char** tmpBuff = malloc(1024);
        tmpBuff = readMessage(buff, tmpBuff); // Filter the client input

        start_mat(countArg, tmpBuff, client); // Run the matinv algorithm
        free(tmpBuff); // Free tmpBuff

        return 0; // Done
    }
    else { // Faulty command provided
        return -1;
    }
    return 0;
}

void sendFile(int pID, int size, char* path){
    char* data = malloc(size);
    FILE* f; // New file descriptor
    f = fopen(path, "r"); // Open 'path' in read mode

    while(fgets(data, size, f) != NULL){ // As long as lines are found, send them to client
        if(send(clientSocket, data, size, 0) == -1){
            perror("Sending data failed");
            bzero(data, size);
            exit(1);
        }
        bzero(data, size); // Clear data every iteration, making place for new data from file
    }
    fclose(f); // Close file descriptor
    free(data);

    send(clientSocket, "\n.\n", 4, 0); // Data transmission completed

    if (remove(path) == 0){ // Remove file after sending the data to the client
        printf("Solution file deleted successfully!\n");
    } else {
        printf("Failed to delete solution file!\n");
    }
}

int recFile(int pID, int size, char* path){
    char* data = malloc(size);
    FILE* f; // New file descriptor
    f = fopen(path, "w"); // Open file 'path' in write-mode
    int n=0; 
    int iteration =0;

    while(1){
        n = recv(clientSocket, data, size, 0); // Receive n bytes of data
        iteration++;
        fflush(stdin);
        if (strcmp(data, "\n.\n") ==0 && n ==4) // When transmission is complete
        {
            iteration--;
            bzero(data, size);
            break;
        }
        fprintf(f, "%s", data); // Write data to file
        bzero(data, size); // Clear data
    }
    fclose(f); // Close file
    free(data);

    return iteration; // Return number of lines in received file
}

char** readMessage(char* buff, char** tmpBuff){
    char* newBuff = malloc(1024);
    int j=0;

    // Global ints
    arg=0;
    countArg=0;
    for (int i=0; i<1024; i++)          
    {
        newBuff[j] = buff[i]; // Copy char from buff to newBuff
        j++;
        if(buff[i] == '\n'){ // If the char in buff[i] is a string terminator the command-filtering is done
            tmpBuff[arg] = (char*)malloc(strlen(newBuff)+1);
            strcpy(tmpBuff[arg], newBuff); // Copy the entirety of stored data to tmpBuff for return
            arg++;
            countArg++;

            free(newBuff);
            break;
        }
        if (buff[i] == ' '){ // If whitespace is found, replace it with '\0'
            countArg++;
            tmpBuff[arg] = (char*)malloc(strlen(newBuff)+1);
            newBuff[j] = '\0';
            strcpy(tmpBuff[arg], newBuff);
            arg++;
            j=0;
            free(newBuff);
            newBuff = malloc(1024);
        }
            
    }
    return tmpBuff; // Return char** containing the fully filtered client command
}

char* getK(char** tmpBuff, char *k){
    int flag=0;
    for(int i=0; i<arg; i++){
        if(strncmp(tmpBuff[i],"-k", 2) == 0){
            k = tmpBuff[i+1];
            flag=1;
            break;
        }
    }
    if(!flag){
        //Assume k=9 if no argument
        strcpy(k, "9");
    }
    return k;

}
