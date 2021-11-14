
#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define ADDR "127.0.0.1"
#define PORT 23
#define SIZE_COM 500

int cli_open();
void cli_close();
char* cli_recv(char*);
int cli_send(const char*);

#endif
