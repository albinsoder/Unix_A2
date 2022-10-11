#ifndef ConnectServer
#define ConnectServer
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "matrix_inverse.h"
#include "kmeans.h"

// Int setup
int sockfd, ret, cnt, commandRes;

// Server socket address structures
struct sockaddr_in serverAddr;

// Struct using stat to check if server_results & server_input exists
struct stat dirCreator;

// Client socket id
int clientSocket;

// Client socket address structures
struct sockaddr_in cliAddr;

// Stores server socket address
socklen_t addr_size;
char* buffer;

// Child process id
pid_t childpid;
char *firstArg;
char* copyBuffer;
int countArg;
// Setup server socket
void initialize(int port);
// Interface waiting for commands from clients
void serverInterface();
// Filter input from client, check if kmeans or matinv is to be run
// and perform the asked for calculations
int readBuffer(char* buff, char k[1024], int N, char* path);
// Send matinv-file to client
void sendFile(int pID, int size, char* path);
char** readMessage(char* buff);
int recFile(int pID, int size, char* path);

#endif