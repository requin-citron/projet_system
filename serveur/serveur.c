
#include "serveur.h"


static int sock;
static int csock[NB_SUPPORT_USERS];
static int id_csock = 0;

clientArray* createClientArray(size_t max_client){
  clientArray* ret = malloc(sizeof(clientArray));
  if(ret==NULL){fprintf(stderr,"%s\n", strerror(errno));exit(errno);}
  ret->lst = malloc(sizeof(FILE *)*max_client);
  if(ret->lst == NULL){fprintf(stderr,"%s\n", strerror(errno));exit(errno);}
  ret->size = max_client;
  ret->end = 0;
  return ret;
}
void freeClientArray(clientArray *in){
  free(in->lst);
  free(in);
  return;
}
int ser_open() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        fprintf(stderr,"%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    struct sockaddr_in sin;
    socklen_t recsize = sizeof(sin);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    if (bind(sock, (struct sockaddr*)&sin, recsize) == -1) {
        fprintf(stderr,"%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if (listen(sock, NB_SUPPORT_USERS) == -1) {
        fprintf(stderr,"%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void ser_close() {close(sock);}
int ser_accept(clientArray *clients) {
    if (id_csock==NB_SUPPORT_USERS) return EXIT_FAILURE;
    struct sockaddr_in csin;
    socklen_t crecsize = sizeof(csin);
    int tmp = accept(sock, (struct sockaddr*)&csin, &crecsize);
    csock[id_csock++] = tmp;
    clients->lst[clients->end] = fdopen(tmp,"a+");
    if(clients->lst[clients->end] == NULL){
      fprintf(stderr,"%s\n", strerror(errno));
      return EXIT_FAILURE;
    }
    setvbuf(clients->lst[clients->end], NULL, _IONBF, 0);
    clients->end++;
    return EXIT_SUCCESS;
}
char* ser_recv(int id, char msg[SIZE_COM]) {
    if (id>=id_csock) return NULL;
    recv(csock[id],msg,SIZE_COM,0);
    return msg;
}
int ser_send(int id, const void* msg) {
    if (id>=id_csock) return EXIT_FAILURE;
    int r = send(csock[id],msg,SIZE_COM,0);
    //printf("%d-%d-%d\n",id,r,((int*)msg)[0]);
    if (r == -1) {
        fprintf(stderr,"%s\n", strerror(errno));
        //printf("ERR: transmission\n");
        return EXIT_FAILURE;
    }
    return r;
}
void ser_sendAll(const void* msg) {
    for (int t=0; t<id_csock; t++)
        ser_send(t,msg);
}
