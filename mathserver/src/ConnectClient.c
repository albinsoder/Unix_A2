#include "../include/ConnectClient.h"

void initialize(int port, char* ip){
    struct stat dirCreator = {0};
    // Create needed directories for input and output data
    if (stat("../../computed_results", &dirCreator) == -1) {
        mkdir("../../computed_results", 0777);
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
 
    if (clientSocket < 0) {
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Client Socket is created.\n");
 
    // Initializing socket structure
    memset(&cliAddr, '\0', sizeof(cliAddr));
 
    // Initializing buffer
    memset(buffer, '\0', sizeof(buffer));
 
    // Assigning port number and IP address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

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
    int helpFlag = 0;
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
    // printf("Client%s: ", clientNmbr);

    newMsg = NULL;
    while (connected) {
        char path[70]; // Array holding path+filename of result file
        matrixFileCount = 0;
        kmeansFileCount = 0;
        int size = 4096;
        char** retBuff = malloc(1024);

        printf("Client%s: ", clientNmbr);
        //Get line of input from user
        getline(&newMsg, &len, stdin);
        //Send to server
        send(clientSocket, newMsg,strlen(newMsg), 0);
        //Flush stdin
        fflush(stdin);
        //Counting the amount of files
        if(fileCounter() == -1){
            break;
        }
        //Read input from user
        retBuff = readMessage(newMsg, retBuff);
        // bzero(newMsg);
        if(strncmp(retBuff[0], "kmeans",6)==0){
            int flag=0;
            char* path_input = malloc(70);


            //Check if using command -f for file
            if(arg >= 3){
                for(int i=0; i<arg; i++){
                    if(strncmp(retBuff[i],"-f", 2) == 0){
                        path_input = retBuff[i+1];
                        flag=1;
                        break;
                    }
                }
            }
            //If no file use standard kmeans-data.txt
            size=1024;
            if(!flag){
                strcpy(path_input, "kmeans-data.txt");
            }
            sendFile(size, path_input, retBuff);

            free(path_input);
            
            //Combine resultfile with respective number, then recieve
            sprintf(path, "../../computed_results/kmeans%s_soln%d.txt", clientNmbr,kmeansFileCount);
            recFile(size, path);

        }
        else if (strncmp(retBuff[0], "matinvpar",9)==0) {
            if (arg >= 2)
            {
                for(int i=0; i<arg; i++){
                    if(strncmp(retBuff[i],"-h", 2) == 0 || strncmp(retBuff[i],"-u", 2) == 0){
                        helpFlag = 1;
                        break;
                    }
                }
            }
            sprintf(path, "../../computed_results/matinv_client%s_soln%d.txt", clientNmbr,matrixFileCount);
            recFile(size, path);
            if (helpFlag == 1)
            {
                helpMessage(path);
            }
        }
        else{
            printf("WRONG INPUT\n");
            printf("Available options:\n");
            printf("[kmeans]\n");
            printf("[matinvpar]\n");
        }
        free(retBuff);
        bzero(path, sizeof(path));
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
            exit(0);
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
        fflush(stdin);
        if (strcmp(data, "\n.\n")==0 || n==4)
        {   
            printf("Succesfuly recieved\n");
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
    return tmpBuff;
}

int fileCounter(){
    DIR *directory;
    int fileValue = 8; // Value of dir entry being a file
    directory = opendir("../../computed_results");
    if (directory == NULL) // If directory could not be opened
    {
        printf("Could not open computed_results directory \n");
        return -1;
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
        return -1;
    }
    return 0;

}

void helpMessage(char* path){
    char textBuff[70];
    FILE *rf;
    rf = fopen(path, "r");
    if (rf == NULL)
    {
        printf("Help-file cannot be opened");
    }
    while (fgets(textBuff, 70, rf) != NULL)
    {
        printf("%s", textBuff);
    }
    fclose(rf);
}