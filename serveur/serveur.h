
#ifndef SERVEUR_H
#define SERVEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 23
#define SIZE_COM 500
#define NB_SUPPORT_USERS 5

int ser_open();
void ser_close();
int ser_accept();
char* ser_recv(int);
int ser_send(int,const char*);

#endif
