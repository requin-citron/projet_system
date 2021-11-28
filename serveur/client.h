#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>

#define FATAL(){fprintf(stderr,"%s\n",strerror(errno));exit(errno);}

#define SIZE_NAME 50
// attention il faut prendre en compte le faite qu'on utilise des char pour stocker
// les carte il faut passer sur du unsigned ou des int pour des plus gros packet
#define SIZE_PACKET 100

typedef struct{
  char name[SIZE_NAME];
  char *cartes;
  FILE *file_ptr;
  size_t size;
  size_t temp;
  size_t prevTime;
  size_t nbFails;
  size_t nbCoupJoue;
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
void clientPrint(client *, FILE *);
bool checkAllClientEmpty(clientArray *);
void clientDelCard(client *, size_t);
void changeAllClientIONonBlock(clientArray *);
void changeAllClientIOBlock(clientArray *);
int createPdf(char *,char *, clientArray *);

#endif
