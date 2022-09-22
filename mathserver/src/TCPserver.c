// Main file for mathserver
// @authors David Värmfors & Albin Södervall
#include "../include/ConnectServer.h"

// Main function, running the server
int main(int argc, char *argv[]) {
    int port;
    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'p'){
        port = atoi(argv[2]);
        // printf("%s, %i \n", argv[2], port);
    }
    else {
        printf("Port was not provided, assuming standard port: 5000\n");
        port = 5000;
        // printf("\n%i \n", port);
    }
    initialize(port);
    serverInterface();

    return 0;
}
