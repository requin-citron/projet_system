#include "client.h"

static int sock;

int cli_open() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    struct sockaddr_in sin;
    sin.sin_addr.s_addr = inet_addr(ADDR);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    if(connect(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void cli_close() {close(sock);}
void* cli_recv(void* msg) {
    recv(sock,msg,SIZE_COM,0);
    return msg;
}
int cli_send(const char msg[SIZE_COM]) {
    int r = send(sock,msg,SIZE_COM,0);
    if (r == -1) {
        //printf("ERR: transmission\n");
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    return r;
}
