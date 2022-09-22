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


int clientSocket, ret;
size_t len;

// Client socket structure
struct sockaddr_in cliAddr, serverAddr;

// char array to store incoming message
char buffer[1024];
char* newMsg;

void initialize(int port, char* ip);
void connectToServer(char clientNr);
void clientInterface();


#endif