
#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define ADDR "127.0.0.1"
#define PORT 23
#define SIZE_COM 500

int cli_open();
void cli_close();
void* cli_recv(void*);
int cli_send(const char[SIZE_COM]);

#endif
