
#include "client.h"

int sock;
char cli_BUFF[SIZE_COM];

int cli_open() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("ERR: socket\n");
        return EXIT_FAILURE;
    }
    struct sockaddr_in sin;
    sin.sin_addr.s_addr = inet_addr(ADDR);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    if(connect(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
        printf("ERR: connect failed\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void cli_close() {close(sock);}
char* cli_recv() {
    recv(sock,cli_BUFF,SIZE_COM,0);
    return cli_BUFF;
}
int cli_send(const char *msg) {
    int r = send(sock,msg,SIZE_COM,0);
    if (r == -1) {
        printf("ERR: transmission\n");
        return EXIT_FAILURE;
    }
    return r;
}
