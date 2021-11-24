#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include "client.h"

#define FATAL(){fprintf(stderr,"%s\n",strerror(errno));exit(errno);}

#define PORT 8080




int createSock();
clientArray* createClientArray(size_t);
void freeClientArray(clientArray *);
void acceptClient(int,clientArray *, size_t);
void *pthreadInitClient(void *);
void *pthreadAskClient(void *);
bool checkNewGame(clientArray *);
#endif
