#include "../include/ConnectClient.h"

void initialize(int port, char* ip){
    struct stat dirCreator = {0};
    // Create needed directories for input and output data
    if (stat("../../computed_results", &dirCreator) == -1) {
        mkdir("../../computed_results", 0777);
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0); // Create client socket
 
    if (clientSocket < 0) {
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Client Socket is created.\n");
 
    // Initialize the socket struct
    memset(&cliAddr, '\0', sizeof(cliAddr));
 
    // Initialize original buffer with '\0' on every index initially
    memset(buffer, '\0', sizeof(buffer));
 
    // Configure the port number and IP address used to connect to the server
    serverAddr.sin_family = AF_INET; // Use IPv4
    serverAddr.sin_port = htons(port); // Set port
    serverAddr.sin_addr.s_addr = inet_addr(ip); // Set IP
};

void connectToServer(){
    // Try to establish connection to server
    ret = connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
 
    if (ret < 0) { // Connection attempt failed
        printf("Error in connection.\n");
        exit(1);
    }

    printf("Connected to Server.\n");
}

void clientInterface(){
    int connected =1;
    int helpFlag = 0; // If client commanded a help command from the matrix inverse
    char clientNmbr[4]; // Connection ID of the client on the server side
    if (recv(clientSocket, buffer, 1024, 0)< 0) { // Receive handshake msg from server
        printf("Error in receiving data.\n");
    }
    else {
        printf("Client connected as: %s\n", buffer);
        strncpy(clientNmbr, buffer, 4);
        bzero(buffer, sizeof(buffer));
    }

    newMsg = NULL;
    while (connected) {
        char path[70]; // Array holding path+filename of result file
        matrixFileCount = 0; // Number of matrix result files
        kmeansFileCount = 0; // Number of kmeans result files
        int size = 4096;
        char** retBuff = malloc(1024);

        printf("Client%s: ", clientNmbr);
        // Get line of input from user
        getline(&newMsg, &len, stdin);
        // Send command to server
        send(clientSocket, newMsg,strlen(newMsg), 0);
        // Flush stdin
        fflush(stdin);
        // Counting the amount of result files
        if(fileCounter() == -1){
            break;
        }
        // Read input from user
        retBuff = readMessage(newMsg, retBuff);
        if(strncmp(retBuff[0], "kmeanspar",9)==0){ // If client commanded kmeans
            int flag=0;
            char* pathInput = malloc(70);
            //Check if using command -f for file
            if(arg >= 3){
                for(int i=0; i<arg; i++){
                    if(strncmp(retBuff[i],"-f", 2) == 0){
                        pathInput = retBuff[i+1];
                        flag=1;
                        break;
                    }
                }
            }
            //If no input file were provided, use standard kmeans-data.txt
            size=1024;
            if(!flag){
                strcpy(pathInput, "kmeans-data.txt");
            }
            sendFile(size, pathInput); // Send input file to server
            free(pathInput);

            // Create new result file and receive resulting data from server and write to the result file
            sprintf(path, "../../computed_results/kmeans%s_soln%d.txt", clientNmbr,kmeansFileCount);
            recFile(size, path);
        }
        else if (strncmp(retBuff[0], "matinvpar",9)==0) { // If matinv was commanded by client
            // Check if client commanded for any of the non-help commands
            helpFlag=1;
            if (arg >= 2)
            {
                for(int i=0; i<arg; i++){
                    if(strncmp(retBuff[i],"-n", 2) == 0 || strncmp(retBuff[i],"-I", 2) == 0 || strncmp(retBuff[i],"-P", 2) == 0 || strncmp(retBuff[i],"-m", 2) == 0){
                        helpFlag = 0;
                        break;
                    }
                }
            }
            else if(arg == 1){ // If only "matinvpar" was entered
                helpFlag = 0;
            }
            // Create new result file and receive resulting data from server and write to the result file
            sprintf(path, "../../computed_results/matinv_client%s_soln%d.txt", clientNmbr,matrixFileCount);
            recFile(size, path);
            if (helpFlag == 1) // If any of the help commands were used, print contents of result file to client
            {
                helpMessage(path);
                // helpFlag=1;
            }
        }
        else{ // If neither kmeanspar or matinvpar was entered by client
            printf("WRONG INPUT/HELP\n");
            printf("Available options:\n");
            printf("[kmeanspar]\n");
            printf("[matinvpar]\n");
        }
        free(retBuff);
        bzero(path, sizeof(path));
    }
    close(clientSocket); // Close socket when connection is lost
 }

void sendFile(int size, char* path){
    char* data = malloc(size);
    int i=0;
    FILE* f; // File descriptor
    f = fopen(path, "r");
    while(fgets(data, size, f) != NULL){ // Send data to server
        if(send(clientSocket, data, size, 0) == -1){
            perror("Sending data failed");
            exit(0);
        }
        bzero(data, size);
    }
    send(clientSocket, "\n.\n", 4, 0); // Data transmission completed
    fclose(f); // Close file descriptor 
    free(data);
}

int recFile(int size, char* path){
    char* data = malloc(size);
    FILE* f; // New file descriptor
    f = fopen(path, "w");
    int n;
    while(1){ // Receive n bytes of data and write to result file
        n = recv(clientSocket, data, size, 0); // Receive data
        fflush(stdin);
        if (strcmp(data, "\n.\n")==0 || n==4) // Check if transmission is completed
        {   
            printf("Received the solution: %s\n", path);
            break;
        }
        fprintf(f, "%s", data); // Write data to file
        bzero(data, size); // Clear data buffer
    }
    fclose(f); // Close file
    free(data);
    return 0;
}

char** readMessage(char* buff, char** tmpBuff){
    char* newBuff = malloc(1024);
    int j;
    j=0;
    arg=0; // Keeps count of entered arguments
    for (int i=0; i<1024; i++)          
    {
        newBuff[j] = buff[i];
        j++;
        // printf("N: %s \n", newBuff);
        if(buff[i] == '\n'){ // If newline is found, end of command is found
            tmpBuff[arg] = (char*)malloc(strlen(newBuff)+1);
            newBuff[j-1] = '\0'; // Let null termination take the newlines place
            strcpy(tmpBuff[arg], newBuff); // Copy filtered arg to tmpBuff
            arg++;
            free(newBuff);
            break;
        }
        if (buff[i] == ' '){ // If whitespace is found, the end of a parameter has been found
            tmpBuff[arg] = (char*)malloc(strlen(newBuff)+1);
            newBuff[j-1] = '\0'; // Add null termination to the parameter
            strcpy(tmpBuff[arg], newBuff); // Copy filtered arg to tmpBuff
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
        if (entry->d_type == fileValue && entry->d_name[0] == 'm') // Number of matrix solution files
        {
            matrixFileCount++;
        }
        else if (entry->d_type == fileValue && entry->d_name[0] == 'k'){ // Number of kmeans solution files
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
    char textBuff[80];
    FILE *rf; // New file descriptor
    rf = fopen(path, "r");
    if (rf == NULL)
    {
        printf("Help-file cannot be opened");
    }
    while (fgets(textBuff, 70, rf) != NULL) // Print help text to client
    {
        printf("%s", textBuff);
    }
    fclose(rf);
}
