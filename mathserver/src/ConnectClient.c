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
    if (recv(clientSocket, buffer, 1024, 0)< 0) {
        printf("Error in receiving data.\n");
    }
    else {
        printf("Client connected as: %s\n", buffer);
        strncpy(clientNmbr, buffer, 4);
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
        DIR *directory;
        int matrixFileCount = 0;
        int kmeansFileCount = 0;
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
        char** retBuff;
        retBuff = readMessage(newMsg);
        if(retBuff[0][0] == 'k'){
            char* data = malloc(size);
            FILE* f;
            f = fopen("kmeans-data.txt", "r");
            while(fgets(data, size, f) != NULL){
                printf("%s \n", data);
                if(send(clientSocket, data, size, 0) == -1){
                    perror("Sending data failed");
                    exit(1);
                }
                bzero(data, size);
            }
            int i=0;
            fclose(f);
            send(clientSocket, "", 1, 0); // Data transmission completed
            // fflush(stdout);
            // printf("VILL HA K: %s \n", retBuff[2]);
            while(1){
                i++;
                printf("%s \n", retBuff[i]);
                if(strncmp(retBuff[i],"-k", 2) == 0){
                    char* k = malloc(1024);
                    k = retBuff[i+1];
                    send(clientSocket, k, 1024, 0);
                    free(k);
                    break;
                }
            }

            // send(clientSocket, fp,sizeof(fp), 0);
            free(data);        }
        else {
            sprintf(path, "../../computed_results/matinv_client%s_soln%d.txt", clientNmbr,matrixFileCount);
            fp = fopen(path, "w");
            int n;

            while(1){
                n = recv(clientSocket, data, size, 0);
                // printf("%s \n", data);
                // printf("Tjabba \n");
                // printf("N: %d \n", n);
                fflush(stdout);
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
        }

        // puts(buffer);
        // bzero(buffer, sizeof(buffer));
        // Printing the message on screen
    }
    close(clientSocket);

 }
char** readMessage(char* buff){
    char** tmpBuff = (char**)malloc(1024);
    char* newBuff = malloc(1024);
    int j,len,countArg;
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