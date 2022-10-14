#include "../include/ConnectClient.h"

void initialize(int port, char* ip){
    struct stat dirCreator = {0};
    // Create needed directories for input and output data
    if (stat("../../computed_results", &dirCreator) == -1) {
        mkdir("../../computed_results", 0777);
    }

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
    char clientNmbr[4];
    unsigned int c_nr;
    if (recv(clientSocket, buffer, 1024, 0)< 0) {
        printf("Error in receiving data.\n");
    }
    else {
        printf("Client connected as: %s\n", buffer);
        strncpy(clientNmbr, buffer, 4);
        c_nr = (u_int64_t)clientNmbr;
        bzero(buffer, sizeof(buffer));
    }
    newMsg = NULL;
    while (connected == 1) {

        getline(&newMsg, &len, stdin);
        send(clientSocket, newMsg,strlen(newMsg), 0);
        fflush(stdin);
        // recv() receives the message
        // from server and stores in buffer
        int matrixFileCount = 0;
        int kmeansFileCount = 0;
        FILE* fp;
        int size = 4096;
        char* data = malloc(size);
        DIR *directory;
        int fileValue = 8; // Value of dir entry being a file
        char path[70]; // Array holding path+filename of result file
        directory = opendir("../../computed_results");
        if (directory == NULL) // If directory could not be opened
        {
            printf("Could not open computed_results directory \n");
            break;
        }
        while ((entry = readdir(directory)) != NULL) // Check all files in computed_results dir
        {
            if (entry->d_type == fileValue && entry->d_name[0] == 'm')
            {
                matrixFileCount++;
            }
            else if (entry->d_type == fileValue && entry->d_name[0] == 'k'){
                kmeansFileCount++;
            }
        }
        if (closedir(directory) == -1)
        {
            printf("Failed to close directory\n");
            break;
        }
        char** retBuff = malloc(1024);
        retBuff = readMessage(newMsg, retBuff);
        if(retBuff[0][0] == 'k'){
            char* data = malloc(size);
            FILE* f;
            char* path_input = malloc(70);
            int flag=0;
            bzero(path, sizeof(path));
            // printf("ARG: %d \n", arg);
            if(arg >= 3){
                for(int i=0; i<arg; i++){
                    if(strncmp(retBuff[i],"-f", 2) == 0){
                        printf("retBuff: %s \n", retBuff[i+1]);
                        // strcpy(path_input, retBuff[i+1]);
                        path_input = retBuff[i+1];
                        flag=1;
                        break;
                    }
                }
            }
            if(!flag){
                strcpy(path_input, "kmeans-data.txt");
            }
            // printf("PATH: %s, SIZE: %d \n", path_input, size);
            sendFile(size, path_input, retBuff);
            // printf("PATH2: \n");
            free(path_input);
            // bzero(path, sizeof(path));
            sprintf(path, "../../computed_results/kmeans%s_soln%d.txt", clientNmbr,kmeansFileCount);
            recFile(size, path);

            // send(clientSocket, fp,sizeof(fp), 0);
            free(data);        
        }
        else {
            sprintf(path, "../../computed_results/matinv_client%s_soln%d.txt", clientNmbr,matrixFileCount);
            recFile(size, path);
        }
        free(retBuff);

        // puts(buffer);
        // bzero(buffer, sizeof(buffer));
        // Printing the message on screen
    }
    close(clientSocket);

 }

void sendFile(int size, char* path, char** retBuff){
    char* data = malloc(size);
    int i=0;
    FILE* f;
    f = fopen(path, "r");
    while(fgets(data, size, f) != NULL){
        // printf("%s \n", data);
        if(send(clientSocket, data, size, 0) == -1){
            perror("Sending data failed");
            exit(1);
        }
        bzero(data, size);
    }
    send(clientSocket, "\n.\n", 4, 0); // Data transmission completed
    fclose(f);
    free(data);
}

int recFile(int size, char* path){
    char* data = malloc(size);
    FILE* f;
    f = fopen(path, "w");
    int n;
    while(1){
        n = recv(clientSocket, data, size, 0);
        // printf("DATA: %s", data);        
        if (strcmp(data, "\n.\n")==0)
        {   
            printf("KLARA NU DED\n");
            break;
        }
        fprintf(f, "%s", data);
        bzero(data, size);
    }
    fclose(f);
    free(data);
    return 0;

}

char** readMessage(char* buff, char** tmpBuff){
    char* newBuff = malloc(1024);
    int j,countArg;
    j=0;
    arg=0;
    countArg=0;
    for (int i=0; i<1024; i++)          
    {
        newBuff[j] = buff[i];
        j++;
        if(buff[i] == '\n'){
            tmpBuff[arg] = (char*)malloc(strlen(newBuff)+1);
            newBuff[j-1] = '\0';
            strcpy(tmpBuff[arg], newBuff);
            arg++;
            free(newBuff);
            break;
        }
        if (buff[i] == ' '){
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
    // free(newBuff);
    return tmpBuff;
    // free(tmpBuff);
}