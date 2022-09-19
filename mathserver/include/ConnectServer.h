#ifndef ConnectServer
#define ConnectServer
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAXLINE 1000

char buffer[100];
int listenfd, len;
struct sockaddr_in servaddr, cliaddr;
int connected;

void initialize(int port);
void serverInterface();



#endif