// Main file for mathserver
// @authors David Värmfors & Albin Södervall
// #include "matrix_inverse.c"
#include <stdio.h>
#include <string.h>
// Main function, running the server
int main(int argc, char **argv) {

    // initialize(5000,"127.0.0.1");
    // connectToServer(1);
    // clientInterface();

    for(int i=0; i< argc; i++){
            printf("%c\n", argv[0][2]);
    }

    // char buffer[1024] = "matinvers -P 1";

    // char *p;

    // p= strtok(buffer, " ");
    // printf("%s", p);

        // if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'p'){
    //     int port = atoi(argv[2]);
    //     printf("%d %s", port, argv[1]);
    // }
    // else {
    //     printf("No port was provided or input was faulty");
    //     return 1;
    // }
    // printf("Köp mer SBB grabbar, TOO THE MOON!");
    return 0;
}