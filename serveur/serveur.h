#ifndef SERVEUR_H
#define SERVEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define PORT 8080
#define SIZE_COM 500
#define NB_SUPPORT_USERS 5


typedef struct{
  FILE **lst;
  size_t end;
  size_t size;
} clientArray;


clientArray* createClientArray(size_t);
void freeClientArray(clientArray *);
int ser_open();
void ser_close();
int ser_accept(clientArray *);
char* ser_recv(int,char[SIZE_COM]);
int ser_send(int, const void*);
void ser_sendAll(const void*);

#endif
