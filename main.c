//http://sdz.tdct.org/sdz/les-sockets.html
//https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "serveur.h"
#include "client.h"
#include "tools.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    char resp[SIZE_IN];
    ask(" Hello, do you want to join a room or create one ? (j or C)\n&> ",resp);
    if (isWord("join",resp)) runCli();
    else runSer();
    printf("\n");
    return EXIT_SUCCESS;
}
