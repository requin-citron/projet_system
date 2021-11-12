
#include "serveur.h"

int runSer() {
    if (openSer()==EXIT_FAILURE) {
        printf("ERR: openSer\n");
        return EXIT_FAILURE;
    }
    nb = 0;
    char resp[SIZE_IN];
    do {
        snprintf(resp,SIZE_IN,"%s","The menu:\n");
        color(resp,RED,FALLBACK);
        printf(" %s",resp);
        printf(" - Start the game (s)\n - Start the game with some bots (b)"
               "\n - Accept Player (a)\n");
        if (nb>0) {
            snprintf(resp,SIZE_IN,"%s","Currently in a room:\n");
            color(resp,BLUE,FALLBACK);
            printf(" %s",resp);
            for (int t=0; t<nb; t++)
                printf(" - %s\n",name[t]);
        }
        ask(" Your choice ?"
            " (S, b or a)\n&> ",resp);
        if (isWord("bot",resp)) {
            // TODO: add bot
            printf("--bot\n");
        }
        else if (isWord("accept",resp)) {
            struct sockaddr_in csin;
            socklen_t crecsize = sizeof(csin);
            csock[nb] = accept(sock, (struct sockaddr*)&csin, &crecsize);
            char buff[SIZE_COM];
            snprintf(buff,SIZE_COM,"%s"," Hello, what's your name ?");
            if (send(csock[nb],buff,SIZE_COM,0) == -1) {
                printf("ERR: transmission/send");
                continue;
            }
            if(recv(csock[nb],name[nb],SIZE_COM,0) == -1) {
                printf("ERR: transmission/recv");
                continue;
            }
            name[nb][strlen(name[nb])-1] = '\0';
            color(name[nb],GREEN,FALLBACK);
            printf(" Player %s is in a room\n",name[nb++]);
        }
    } while (isWord("accept",resp)&&nb<NB_SUPPORT_PLAYER);

    gameSer();

    while (nb-->=0) {
        shutdown(csock[nb], SHUT_RDWR);
        close(csock[nb]);
    }
    closeSer();
    return EXIT_SUCCESS;
}
int openSer() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("ERR: socket\n");
        return EXIT_FAILURE;
    }
    struct sockaddr_in sin;
    socklen_t recsize = sizeof(sin);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    if (bind(sock, (struct sockaddr*)&sin, recsize) == -1) {
        printf("ERR: bind\n");
        close(sock);
        return EXIT_FAILURE;
    }
    if (listen(sock, NB_SUPPORT_PLAYER) == -1) {
        printf("ERR: listen\n");
        close(sock);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void closeSer() {close(sock);}
void gameSer() {
    char resp[SIZE_COM];
    snprintf(resp,SIZE_COM,
        " The Game begins !!\n We are %d in a room.\n -> %sMaster%s\n",nb+1,GREEN,FALLBACK);
    for (int t=0; t<nb; t++) {
        char tmp[SIZE_COM];
        strcpy(tmp,resp);
        snprintf(resp,SIZE_COM,"%s -> %s\n",tmp,name[t]);
    }
    printf(resp);
    for (int t=0; t<nb; t++)
        send(csock[t],resp,SIZE_COM,0);
    int cards[NB_CARD];
    int allCards[100];
    initListCards(allCards);
    for (int t=0; t<nb; t++) {
        pickCards(allCards,cards);
        send(csock[t],cards,sizeof(cards),0);
    }
    pickCards(allCards,cards);
    printf(" Your Cards : ");
    printCards(cards);
    printf("\n");

}
