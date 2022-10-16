// Main file for client
// @authors David Värmfors & Albin Södervall
#include "../include/ConnectClient.h"

// Main function, running the client
int main(int argc, char **argv) {
    //Default values
    char* ip = "127.0.0.1"; 
    int port = 5000;
    int ip_flag=0;
    int p_flag=0;

    while (++argv, --argc > 0)
        if (**argv == '-')
            switch (*++ * argv) {
            case 'h':
                printf("Commands: \n");
                printf("-p  provide port\n");
                printf("-ip provide ip\n");
                printf("-h  print options\n");
                exit(0);
                break;
            case 'p':
                argc--;
                port = atoi(*++argv);
                p_flag=1;
                break;
            case 'i':
                if(*++ * argv =='p'){
                    argc--;
                    ip = *++argv;
                    ip_flag=1;
                }
                break;
            }


    if(!ip_flag && p_flag){
        printf("Assuming standard address: 127.0.0.1\n");
    }
    else if(ip_flag && !p_flag){
        printf("Assuming standard port: 5000\n");
       
    }

    initialize(port,ip); // Start the server with provided port and IP
    connectToServer(1); // Establish a TCP connection
    clientInterface(); // Interface where the client sends and recieves commands/data

    return 0;
}
