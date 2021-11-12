
#ifndef SERVEUR_H
#define SERVEUR_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "gameTools.h"
#define NB_SUPPORT_PLAYER 5

int runSer();
int openSer();
void gameSer();
void closeSer();

#endif
