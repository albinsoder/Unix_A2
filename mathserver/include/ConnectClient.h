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

// Initialize the client (create socket, needed directoriesm set port and IP)
void initialize(int port, char* ip);
// Connect to the server
void connectToServer(char clientNr);
void clientInterface();
char** readMessage(char* buff, char** tmpBuff);
void sendFile(int size, char* path, char **retBuff);
int recFile(int size, char* path);
int fileCounter();
void helpMessage(char* path);
#endif