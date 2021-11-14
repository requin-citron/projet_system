//http://sdz.tdct.org/sdz/les-sockets.html
//https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serveur.h"
#define SIZE_IN 50

char* ask(const char *intro, char *str) {
    printf("%s&> ",intro);
    fgets(str,SIZE_IN,stdin);
    fflush(stdin);
    return str;
}

int main(int argc, char** argv) {
    ser_open();

    ser_accept();
    ser_send(0," Presente toi salope\n");
    printf(ser_recv(0));
    ser_send(0,"Bonjour\n");

    ser_close();
    printf("\n");
    return EXIT_SUCCESS;
}

void phase_connection() {
    
}
