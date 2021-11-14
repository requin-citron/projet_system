
#include "serveur.h"

static int sock;
static int csock[NB_SUPPORT_USERS];
static int id_csock = 0;

int ser_open() {
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
        return EXIT_FAILURE;
    }
    if (listen(sock, NB_SUPPORT_USERS) == -1) {
        printf("ERR: listen\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void ser_close() {close(sock);}
int ser_accept() {
    if (id_csock==NB_SUPPORT_USERS) return EXIT_FAILURE;
    struct sockaddr_in csin;
    socklen_t crecsize = sizeof(csin);
    csock[id_csock++] = accept(sock, (struct sockaddr*)&csin, &crecsize);
    return EXIT_SUCCESS;
}
char* ser_recv(int id, char* msg) {
    if (id>=id_csock) return NULL;
    recv(csock[id],msg,SIZE_COM,0);
    return msg;
}
int ser_send(int id, const char* msg) {
    if (id>=id_csock) return EXIT_FAILURE;
    int r = send(csock[id],msg,SIZE_COM,0);
    //printf("%d-%d-%s",id,r,msg);
    if (r == -1) {
        printf("ERR: transmission\n");
        return EXIT_FAILURE;
    }
    return r;
}
void ser_sendAll(const char* msg) {
    for (int t=0; t<id_csock; t++)
        ser_send(t,msg);
}
