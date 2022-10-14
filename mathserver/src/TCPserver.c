// Main file for mathserver
// @authors David Värmfors & Albin Södervall
#include "../include/ConnectServer.h"

// Main function, running the server
int main(int argc, char *argv[]) {
    int port;
    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'p'){ // If -p was specified on launch of the server
        port = atoi(argv[2]);
    }
    else {
        printf("Port was not provided, assuming standard port: 5000\n");
        port = 5000;
    }
    initialize(port); // Set up server socket
    serverInterface(); // Interface that accepts commands from client and performs them
    return 0;
}
