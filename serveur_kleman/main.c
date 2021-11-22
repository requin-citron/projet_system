#include <stdio.h>
#include <stdlib.h>
#include "serveur.h"

int main(int argc, char const *argv[]) {
  size_t nb_client;
  int sock = createSock();
  clientArray *lstClient = NULL;
  printf("Combien de client: ");
  //on a le droit car on prend un size_t en input
  scanf("%lu",&nb_client);
  printf("Vous avez choisie %lu clients\n", nb_client);
  lstClient =createClientArray(nb_client);
  acceptClient(sock, lstClient, nb_client);
  for (size_t i = 0; i < lstClient->size; i++) {
    printf("Client N %lu %s\n",i,lstClient->lst[i].name);
  }
  //game loop

  //end game loop
  freeClientArray(lstClient);
  close(sock);
  return 0;
}
