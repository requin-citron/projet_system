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

#define PORT 8080
#define SIZE_NAME 50

typedef struct{
  char name[SIZE_NAME];
  size_t *cartes;
  FILE *file_ptr;
  size_t size;
} client;

typedef struct{
  client *lst;
  size_t size;
} clientArray;



int createSock();
clientArray* createClientArray(size_t);
void freeClientArray(clientArray *);
void acceptClient(int,clientArray *, size_t);
void *pthreadInitClient(void *);
void *pthreadAskClient(void *);
bool checkNewGame(clientArray *);
#endif
