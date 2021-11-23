#include <stdio.h>
#include <stdlib.h>
#include "serveur.h"

int main(int argc, char const *argv[]) {
  if(argc != 2){
    printf("Usage: %s nb_joeur\n", argv[0]);
    return 1;
  }
  size_t nb_client;
  int sock = createSock();
  clientArray *lstClient = NULL;
  nb_client = atol(argv[1]);
  printf("Vous avez choisie %lu clients\n", nb_client);
  lstClient =createClientArray(nb_client);
  acceptClient(sock, lstClient, nb_client);
  for (size_t i = 0; i < lstClient->size; i++) {
    printf("Client N %lu %s\n",i,lstClient->lst[i].name);
  }
  //game loop
  do {
    /* code */
  } while(checkNewGame(lstClient)==true); //check for a new game
  //end game loop
  freeClientArray(lstClient);
  close(sock);
  return 0;
}
