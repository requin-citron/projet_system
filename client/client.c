#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080


int main(int argc, char const *argv[]) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  char buffer[1024];
  struct sockaddr_in servaddr;
  if(sock == -1){
    fprintf(stderr, "%s\n", strerror(errno));
    return errno;
  }
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
  servaddr.sin_port = htons(SERVER_PORT);
  if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0){
    fprintf(stderr, "%s\n",strerror(errno));
    return errno;
  }
  while (true) {
    fgets(buffer, 1024, stdin);
    send(sock, buffer, 1024);
  }
  return 0;
}
