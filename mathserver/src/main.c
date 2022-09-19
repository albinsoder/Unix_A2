// Main file for mathserver
// @authors David Värmfors & Albin Södervall
#include <stdio.h>
#include <stdlib.h>

// Main function, running the server
int main(int argc, char *argv[]) {
    if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'p'){
        int port = atoi(argv[2]);
        printf("%d %s", port, argv[1]);
    }
    else {
        printf("No port was provided or input was faulty");
        return 1;
    }
    printf("Köp mer SBB grabbar, TOO THE MOON!");
    return 0;
}