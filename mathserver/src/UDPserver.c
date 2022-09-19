// Main file for mathserver
// @authors David Värmfors & Albin Södervall
#include "../include/ConnectServer.h"

// Main function, running the server
int main(int argc, char *argv[]) {

    initialize(5000);
    serverInterface();

    return 0;
}