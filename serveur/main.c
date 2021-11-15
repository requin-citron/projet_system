//http://sdz.tdct.org/sdz/les-sockets.html
//https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "serveur.h"
#define SIZE_IN 50

#define NB_CARDS 10

int part_askNbPlayers();
int part_acceptUsers(int,char[NB_SUPPORT_USERS][SIZE_IN]);
void part_distribCards(int,int*);
void part_game(int,char[NB_SUPPORT_USERS][SIZE_IN],int*);
char* ask(const char*,char[SIZE_IN]);
int* pickCards(int[100],int[NB_CARDS]);
int* sort(int[NB_CARDS]);
void printCards(int*,int);
int cardsNotEmpty(int*,int);

int main(int argc, char** argv) {
    while(ser_open() == EXIT_FAILURE) {
      printf("Retry... ");
      getchar();
    }
    fflush(stdin);

    srand(time(NULL));

    int nbPlayers = part_askNbPlayers();

    char namePlayers[nbPlayers][SIZE_IN];
    part_acceptUsers(nbPlayers,namePlayers);

    int minCardsByPlayers[nbPlayers];
    part_distribCards(nbPlayers,minCardsByPlayers);

    part_game(nbPlayers,namePlayers,minCardsByPlayers);

    ser_close();
    printf("\n");
    return EXIT_SUCCESS;
}
int part_askNbPlayers() {
    int nbPlayers = -1;
    do {
        if (nbPlayers==0)
            printf(" Incorrect input error\n");
        printf(" How many players are we expecting ? (<%d)\n",NB_SUPPORT_USERS);
        char buff[SIZE_IN];
        nbPlayers=atoi(ask("",buff));
    } while (nbPlayers==0||nbPlayers>NB_SUPPORT_USERS);
    return nbPlayers;
}
int part_acceptUsers(int nbPlayers, char namePlayers[NB_SUPPORT_USERS][SIZE_IN]) {
    for (int t=0; t<nbPlayers; t++) {
        ser_accept();
        ser_send(t," Hello, what's your name ?\n");
        char buff[SIZE_COM];
        ser_recv(t,buff);
        snprintf(namePlayers[t],SIZE_IN,buff);
        namePlayers[t][strlen(namePlayers[t])-1] = '\0';
        snprintf(buff,SIZE_COM," Player %s (%d/%d) has just entered the room.\n",namePlayers[t],t+1,nbPlayers);
        ser_sendAll(buff);
        printf(buff);
    }
    return nbPlayers;
}
void part_distribCards(int nbPlayers, int* minCardsByPlayers) {
    ser_sendAll("NEXT");
    ser_sendAll(" Welcome in the mind game !\n Your cards are : ");
    int allCards[100];
    for (int t=0; t<100; t++)
        allCards[t] = t+1;
    for (int t=0; t<nbPlayers; t++) {
        int cards[NB_CARDS];
        pickCards(allCards,cards);
        minCardsByPlayers[t] = cards[0];
        ser_send(t,cards);
        printf(" ");
        printCards(cards,NB_CARDS);
        printf("\n");
    }
}
void part_game(int nbP, char nameP[NB_SUPPORT_USERS][SIZE_IN], int* minCars) {
    /*int cardTmp;
    int idTurnP = rand()%nbP;
    do {
        ser_send(idTurnP,"PLAY");
        char buff[SIZE_COM];
        ser_recv(idTurnP,buff);
        if(!strcmp(buff,"P")) {

            cli_recv(buff);
        }
    } while();*/
}
char* ask(const char *intro, char str[SIZE_IN]) {
    printf("%s&> ",intro);
    fgets(str,SIZE_IN,stdin);
    fflush(stdin);
    return str;
}
int* pickCards(int allCards[100], int cards[NB_CARDS]) {
    int nb=0;
    while (nb<NB_CARDS) {
        int pick = rand()%100;
        if (allCards[pick]==-1)
            continue;
        cards[nb++] = allCards[pick];
        allCards[pick] = -1;
    }
    return sort(cards);
}
int* sort(int cards[NB_CARDS]) {
    for (int t=0; t<NB_CARDS-1; t++)
        for (int h=t+1; h<NB_CARDS; h++)
            if (cards[t]>cards[h]) {
                int tmp = cards[t];
                cards[t] = cards[h];
                cards[h] = tmp;
            }
    return cards;
}
void printCards(int* cards, int s) {
    int pos = cardsNotEmpty(cards,s);
    if (pos==-1) return;
    printf("[%d",cards[pos]);
    while (++pos<s)
        if (cards[pos]!=-1)
            printf(",%d",cards[pos]);
    printf("]");
}
int cardsNotEmpty(int *cards, int s) {
    for (int t=0; t<s; t++)
        if (cards[t]!=-1)
            return t;
    return -1;
}
