//http://sdz.tdct.org/sdz/les-sockets.html
//https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#define SIZE_IN 50

char* ask(const char *intro, char *str) {
    printf("%s&> ",intro);
    fgets(str,SIZE_IN,stdin);
    fflush(stdin);
    return str;
}

int main(int argc, char** argv) {
    cli_open();

    char buff[SIZE_COM];
    cli_recv(buff);
    cli_send(ask(buff,buff));
    cli_recv(buff);
    while(strcmp(buff,"NEXT")) {
        printf(buff);
        cli_recv(buff);
    }
    printf("%s\n",cli_recv(buff));


    cli_close();
    printf("\n");
    return EXIT_SUCCESS;
}
