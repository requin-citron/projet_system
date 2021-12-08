#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "serveur.h"
#include "client.h"

#define ITERATION_RANDOM 100
#define SIZE_INPUT_USER 5
#define CURL_OUT 50

size_t write_handler(void *ptr, size_t size, size_t nbemb, void *s){
  strncpy((char *)s, (char *)ptr, CURL_OUT-1);
  return size*nbemb;
}

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
  char prev_card = 0;
  bool manche_state = true;
  size_t currTime;
  //client input
  char client_input[SIZE_INPUT_USER];
  char *ret_fgets = NULL;
  size_t index_card;
  //curl
  CURLcode ret;
  CURL *curl;
  FILE *pdf = NULL;
  struct stat file_info;
  char out[CURL_OUT] = {0};
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
    changeAllClientIONonBlock(lstClient);
    if(nb_manche < curr_manche) curr_manche=1;
    packetFlush(packet, SIZE_PACKET, 1000);

    for (size_t i = 0; i < lstClient->size; i++) {
      fprintf(lstClient->lst[i].file_ptr, "\e[1;1H\e[2JVous etes manche %lu\n",curr_manche);
    }

    sendCards(lstClient,curr_manche, packet);

    while(checkAllClientEmpty(lstClient) && manche_state){
      //for each clients
      for (size_t i = 0; i < lstClient->size && ret_fgets==NULL; i++) {
        ret_fgets= fgets(client_input, SIZE_INPUT_USER,lstClient->lst[i].file_ptr);
        if(ret_fgets!=NULL){
          index_card=strtoul(client_input, NULL, 10);
          //entré invalide
          if(client_input[0]<0x30 || client_input[0]>0x39){
            fprintf(lstClient->lst[i].file_ptr, "Entrée invalide\n");
          }else{

            if(index_card < lstClient->lst[i].size){ // nombre valide
              //envois de l'info a tout le monde
              for(size_t d=0;d<lstClient->size;d++){
                fprintf(lstClient->lst[d].file_ptr, "Joueur [%s] a jouer la carte %d\n", lstClient->lst[i].name, lstClient->lst[i].cartes[index_card]);
              }
              if((prev_card != 0) && (prev_card > lstClient->lst[i].cartes[index_card])){
                manche_state = false;
                lstClient->lst[i].nbFails++;
              }
              prev_card = lstClient->lst[i].cartes[index_card];
              clientDelCard(lstClient->lst + i, index_card);
              clientPrint(lstClient->lst+i, lstClient->lst[i].file_ptr);
              currTime = time(NULL);
              lstClient->lst[i].temp += currTime - lstClient->lst[i].prevTime;
              lstClient->lst[i].prevTime = currTime;
              lstClient->lst[i].nbCoupJoue++;
            }
          }
        }
      }
      ret_fgets=NULL;
      //anti saturation du cpu
      usleep(5000);
    }
    if(manche_state){
      for (size_t i = 0; i < lstClient->size; i++) {
        fprintf(lstClient->lst[i].file_ptr, "Vous avez gagné la manche\n");
      }
      curr_manche++;
    }else{
      for (size_t i = 0; i < lstClient->size; i++) {
        fprintf(lstClient->lst[i].file_ptr, "Vous avez perdu retour manche 1\n");
      }
      curr_manche=1;
    }
    manche_state = true;
    prev_card = 0;
    changeAllClientIOBlock(lstClient);
    sleep(3);
  } while((curr_manche <= nb_manche) || (checkNewGame(lstClient)==true)); //check for a new game
  //end game loop
  if(createPdf("./template/template.tex", "./template/out", lstClient)==0){
      pdf = fopen("./template/out.pdf","rb");
      if(pdf == NULL){
        fprintf(stderr,"%s\n", strerror(errno));
        return errno;
      }
      fstat(fileno(pdf), &file_info);
      //curl things
      curl = curl_easy_init();
      curl_easy_setopt(curl, CURLOPT_URL, "https://transfer.tukif.info/out.pdf");
      curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
      curl_easy_setopt(curl, CURLOPT_READDATA, pdf);
      curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)file_info.st_size);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,write_handler);

      curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)out);
      ret = curl_easy_perform(curl);
      if(ret != CURLE_OK){
        fprintf(stderr,"Curl: %s\n", curl_easy_strerror(ret));
      }
      curl_easy_cleanup(curl);
      for (size_t i=0;i<lstClient->size;i++){
        fprintf(lstClient->lst[i].file_ptr, "Pdf de la partie %s\n", out);
      }
      fclose(pdf);
      pdf=NULL;
  }else{
    for (size_t i=0;i<lstClient->size;i++){
      fprintf(lstClient->lst[i].file_ptr, "Créeation du pdf est un echec\n");
    }
  }
  //clean
  free(packet);
  freeClientArray(lstClient);
  close(sock);
  return 0;
}
