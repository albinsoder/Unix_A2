// Main file for mathserver
// @authors David Värmfors & Albin Södervall
#include "../include/ConnectServer.h"

// Main function, running the server
int main(int argc, char **argv) {
    int port = 5000;
    int p_flag=0;

    while (++argv, --argc > 0)
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
                p_flag=1;
                break;
            }
    

    if(!p_flag){
        printf("Assuming standard port: 5000\n");
    }

    initialize(port); // Set up server socket
    serverInterface(); // Interface that accepts commands from client and performs them
    return 0;
}
