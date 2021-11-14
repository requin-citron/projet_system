//http://sdz.tdct.org/sdz/les-sockets.html
//https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serveur.h"
#define SIZE_IN 50

int part_acceptUsers(char[NB_SUPPORT_USERS][SIZE_IN]);
char* ask(const char*,char*);

int main(int argc, char** argv) {
    ser_open();
    char namePlayers[NB_SUPPORT_USERS][SIZE_IN];
    int nbPlayers = part_acceptUsers(namePlayers);
    ser_sendAll("NEXT");
    ser_sendAll("crotte\n");
    ser_close();
    printf("\n");
    return EXIT_SUCCESS;
}

int part_acceptUsers(char namePlayers[NB_SUPPORT_USERS][SIZE_IN]) {
    int nbPlayers = -1;
    do {
        if (nbPlayers==0)
            printf(" Incorrect input error\n");
        printf(" How many players are we expecting ? (<%d)\n",NB_SUPPORT_USERS);
        char buff[SIZE_IN];
        nbPlayers=atoi(ask("",buff));
    } while (nbPlayers==0||nbPlayers>NB_SUPPORT_USERS);
    for (int t=0; t<nbPlayers; t++) {
        ser_accept();
        ser_send(t," Hello, what's your name ?\n");
        char buff[SIZE_COM];
        ser_recv(t,buff);
        snprintf(namePlayers[t],SIZE_IN,buff);
        namePlayers[t][strlen(namePlayers[t])-1] = '\0';
        snprintf(buff,SIZE_COM," Player %s (%d/%d) has just entered the room\n",namePlayers[t],t+1,nbPlayers);
        ser_sendAll(buff);
    }
    return nbPlayers;
}

char* ask(const char *intro, char *str) {
    printf("%s&> ",intro);
    fgets(str,SIZE_IN,stdin);
    fflush(stdin);
    return str;
}
