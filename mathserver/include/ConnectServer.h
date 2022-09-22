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


// Int setup
int sockfd, ret, cnt;

// Server socket address structures
struct sockaddr_in serverAddr;

// Client socket id
int clientSocket;

// Client socket address structures
struct sockaddr_in cliAddr;

// Stores server socket address
socklen_t addr_size;
char buffer[1024];

// Child process id
pid_t childpid;

// Setup server socket
void initialize(int port);
// Interface waiting for commands from clients
void serverInterface();


#endif