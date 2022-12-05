#ifndef ConnectClient
#define ConnectClient
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


int clientSocket, ret;
int matrixFileCount;
int kmeansFileCount;

size_t len;
int arg;
// Client socket structure
struct sockaddr_in cliAddr, serverAddr;
// Directory struct for reading filenames in specified directory
struct dirent *entry;

// Struct using stat to check if computed_results exists
struct stat dirCreator;

// char array to store incoming message
char buffer[1024];
char* newMsg;

// Initialize the client (create socket, needed directories, set port and IP for server connection)
void initialize(int port, char* ip);
// Connect to the server
void connectToServer();
// Interface for the client sending commands to the server and receiving solutions
void clientInterface();
// Filter the entered command into array
char** readMessage(char* buff, char** tmpBuff);
// Send a file to the server
void sendFile(int size, char* path);
// Receive a file from the server
int recFile(int size, char* path);
// Count number of kmeans and matinvpar solution files currently existing in computed_results dir
int fileCounter();
// Print help text provided from the server when running matrix inverse (-h, -u, -D)
void helpMessage(char* path);
#endif