//http://sdz.tdct.org/sdz/les-sockets.html
//https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#define SIZE_IN 50

#define NB_CARDS 10

char* ask(const char*,char[SIZE_IN]);
void printCards(int*,int);
int cardsNotEmpty(int*,int);

int main(int argc, char** argv) {
    while(cli_open() == EXIT_FAILURE) {
      printf("Retry... ");
      getchar();
    }
    fflush(stdin);
    char buff[SIZE_COM];
    cli_recv(buff); // hello
    cli_send(ask(buff,buff)); // send name
    cli_recv(buff); // players reception
    while(strcmp(buff,"NEXT")) {
        printf(buff);
        cli_recv(buff);
    }
    printf(cli_recv(buff)); // begin game
    int cards[NB_CARDS];
    cli_recv(cards); // recv cards
    printCards(cards,NB_CARDS);



    cli_close();
    printf("\n");
    return EXIT_SUCCESS;
}
char* ask(const char *intro, char str[SIZE_IN]) {
    printf("%s&> ",intro);
    fgets(str,SIZE_IN,stdin);
    fflush(stdin);
    return str;
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
