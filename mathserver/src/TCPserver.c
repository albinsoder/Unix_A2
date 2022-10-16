// Main file for mathserver
// @authors David Värmfors & Albin Södervall
#include "../include/ConnectServer.h"

// Main function, running the server
int main(int argc, char **argv) {
    int port = 5000;
    int pFlag=0;

    while (++argv, --argc > 0) // Check input parameters
        if (**argv == '-')
            switch (*++ * argv) {
            case 'h':
                printf("Commands: \n");
                printf("-p  provide port\n");
                printf("-h  print options\n");
                exit(0);
                break;
            case 'p':
                argc--;
                port = atoi(*++argv);
                pFlag=1;
                break;
            }
    

    if(!pFlag){
        printf("Assuming standard port: 5000\n");
    }

    initialize(port); // Set up server socket
    serverInterface(); // Interface that accepts commands from client and performs them
    return 0;
}
