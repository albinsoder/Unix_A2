// Main file for client
// @authors David Värmfors & Albin Södervall
#include "../include/ConnectClient.h"

// Main function, running the client
int main(int argc, char *argv[]) {
    char* ip[10]; 
    int port;
    if (argc >= 3){ // If three or more arguments was specified on launch, some config is needed
        if (argv[1][0] == '-' && argv[1][1] == 'i' && argv[1][2] == 'p') // If -ip was specified
        {
            ip[0] = argv[2];
            if(argc >= 4 && argv[3][0] == '-' && argv[3][1] == 'p'){ // If -p was also specified
                port = atoi(argv[4]);
            }
            else{ // else assume standard port
                printf("Port was not provided, assuming standard port: 5000\n");
                port = 5000;
            }
        }
        else if(argv[1][0] == '-' && argv[1][1] == 'p'){
            port = atoi(argv[2]);
            if(argc >= 4 && argv[3][0] == '-' && argv[3][1] == 'i' && argv[3][2] == 'p'){
                ip[0] = argv[4];
            }
            else {
                printf("IP was not provided, assuming standard address: 127.0.0.1\n");
                ip[0] = "127.0.0.1";
            }
        }
    }
    else {
        printf("No port or ip was provided or input was faulty\n");
        printf("Assuming standard address: 127.0.0.1\n");
        ip[0] = "127.0.0.1";
        printf("Assuming standard port: 5000\n");
        port = 5000;
    }
    // printf("%s \n", ip[0]);
    // printf("%i", port);
    initialize(port,*ip); // Start the server with provided port and IP
    connectToServer(1); // Establish a TCP connection
    clientInterface(); // Interface where the client sends and recieves commands/data

    return 0;
}
