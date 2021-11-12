
#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "gameTools.h"

int sock;
int runCli();
int openCli();
void gameCli();
void closeCli();

#endif
