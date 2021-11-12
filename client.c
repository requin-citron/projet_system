
#include "client.h"

int runCli() {
    if (openCli()==EXIT_FAILURE) {
        printf("ERR: openCLi\n");
        return EXIT_FAILURE;
    }
    char resp[SIZE_COM];
    while (recv(sock,resp,SIZE_COM,0) == -1);
    printf(resp);
    ask("\n&> ",resp);
    if (send(sock,resp,SIZE_COM,0) == -1) {
        printf("ERR: transmission/send");
        return EXIT_FAILURE;
    }

    gameCli();

    closeCli();
    return EXIT_SUCCESS;
}
void gameCli() {
    printf(" Waiting for more players...\n");
    char resp[SIZE_COM];
    recv(sock,resp,SIZE_COM,0);
    printf(resp);
    int cards[NB_CARD];
    recv(sock,cards,sizeof(cards),0);
    printf(" Your cards : ");
    printCards(cards);
    printf("\n");
}
int openCli() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("ERR: socket\n");
        return EXIT_FAILURE;
    }
    struct sockaddr_in sin;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    if(connect(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
        printf("ERR: connect failed\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void closeCli() {
    close(sock);
}
