#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "serveur.h"
#include "client.h"

#define ITERATION_RANDOM 100
#define SIZE_INPUT_USER 5

int main(int argc, char const *argv[]) {
  if(argc != 3){
    printf("Usage: %s nb_joeur nb_manche\n", argv[0]);
    return 1;
  }
  srand(time(NULL));

  size_t nb_client;
  size_t nb_manche;
  int sock = createSock();
  clientArray *lstClient = NULL;
  char *packet = createPacket(SIZE_PACKET,1000);
  size_t curr_manche=1;
  //client input
  char client_input[SIZE_INPUT_USER];
  char *ret_fgets;
  size_t index_card;

  nb_client = atol(argv[1]);
  nb_manche = atol(argv[2]);
  if(nb_client*nb_manche > SIZE_PACKET){
    puts("le packet n'est pas assez grand pour la configuration de la partie");
    return 1;
  }
  printf("Vous avez choisie %lu clients\n", nb_client);
  lstClient =createClientArray(nb_client);
  acceptClient(sock, lstClient, nb_client);
  for (size_t i = 0; i < lstClient->size; i++) {
    printf("Client N %lu %s\n",i,lstClient->lst[i].name);
  }
  //game loop
  do {
    packetFlush(packet, SIZE_PACKET, 1000);
    sendCards(lstClient,curr_manche, packet);

    changeAllClientIONonBlock(lstClient);
    while(checkAllClientEmpty(lstClient)){
      //for each clients
      for (size_t i = 0; i < lstClient->size; i++) {
        ret_fgets= fgets(client_input, SIZE_INPUT_USER,lstClient->lst[i].file_ptr);
        if(ret_fgets!=NULL){
          index_card=strtoul(client_input, NULL, 10);
          //entré invalide
          if(errno == EINVAL){
            fprintf(lstClient->lst[i].file_ptr, "Entrée invalide\n");
          }else{

            if(index_card < lstClient->lst[i].size){
              //envois de l'info a tout le monde
              for(size_t d=0;d<lstClient->size;d++){
                fprintf(lstClient->lst[d].file_ptr, "Joueur [%s] a jouer la carte %d\n", lstClient->lst[i].name, lstClient->lst[i].cartes[index_card]);
              }
              clientDelCard(lstClient->lst + i, index_card);
              clientPrint(lstClient->lst+i, lstClient->lst[i].file_ptr);
            }

          }
        }
      }
      //anti saturation du cpu
      usleep(5000);
    }
    changeAllClientIOBlock(lstClient);

  } while(checkNewGame(lstClient)==true); //check for a new game
  //end game loop
  //clean
  free(packet);
  freeClientArray(lstClient);
  close(sock);
  return 0;
}
