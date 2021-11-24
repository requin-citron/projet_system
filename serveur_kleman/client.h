#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define FATAL(){fprintf(stderr,"%s\n",strerror(errno));exit(errno);}

#define SIZE_NAME 50
#define SIZE_PACKET 100

typedef struct{
  char name[SIZE_NAME];
  char *cartes;
  FILE *file_ptr;
  size_t size;
} client;

typedef struct{
  client *lst;
  size_t size;
} clientArray;


void packetPrint(char *, size_t);
char *createPacket(size_t, size_t);
void packetFlush(char *, size_t, size_t);
void clientGetCards(client *, char *, size_t);
void sendCards(clientArray *,size_t, char *);
void clientPrint(client *);

#endif
