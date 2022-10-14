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

// Setup needed global ints
int sockfd, ret, cnt, commandRes;

// Set up socket address struct for the server
struct sockaddr_in serverAddr;

// Address structures of the client socket
struct sockaddr_in cliAddr;

// Struct using stat to check if server_results & server_input exists
struct stat dirCreator;

// Socket of client
int clientSocket;

// Store the servers socket address
socklen_t addr_size;
char* buffer;

// Child process id
pid_t childpid;
char *firstArg;
char* copyBuffer;
int countArg;
int len;

// Setup server socket (IP + port)
void initialize(int port);
// Interface waiting for commands from clients
void serverInterface();
// Check input from client, if kmeans or matinv is to be run
// and perform the asked for calculations
int readBuffer(char* buff, char k[1024], int N, char* path);

// Send file to client (connection number, size of buffer, path to file)
void sendFile(int pID, int size, char* path);
// Filter command input, remove whitespaces etc and store in char** array
char** readMessage(char* buff, char** tmpBuff);
// Recieve file (connection number, size of buffer, path to file where recieved data is to be written)
int recFile(int pID, int size, char* path);
// Free allocated char buffers
void freeMessage(char** tmpBuff, char check);

#endif