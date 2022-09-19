#ifndef ConnectClient
#define ConnectClient
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#define MAXLINE 1000

char buffer[100];
char message[100];
int sockfd, n;
struct sockaddr_in servaddr;
int connected; // flag stating if currently connected to server
size_t len;
char *newMsg;


void initialize(int port, char* ip);
void connectToServer(char clientNr);
void clientInterface();


#endif