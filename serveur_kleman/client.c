#include "client.h"

//! fonction de debug
/*!
  \param[in] in list a afficher
  \param[in] len taille de la list
*/
void packetPrint(char *in, size_t len){
  for(size_t i=0; i<len;i++){
    printf("%d,",(int)in[i]);
  }
  puts("");
  return;
}

//! fonction de debug
/*!
  \param[in] cli pointeur sur client
*/
void clientPrint(client *cli){
  printf("Joueur %s :",cli->name);
  for(size_t i=0;i<cli->size;i++){
    printf("%d,",cli->cartes[i]);
  }
  puts("");
}

//! melange le packet
/*!
  \param[out] in tableau a melanger
  \param[in] nbCards taille du tableau
  \param[in] nbRandom nombre de passe
*/
void packetFlush(char *in, size_t nbCards, size_t nbRandom){
  char tmp;
  size_t index1;
  size_t index2;
  for(size_t i=0;i<nbRandom;i++){
    index1=rand()%nbCards;//attention le random n'est pas parfait dans ce cas la
    index2=rand()%nbCards;
    tmp = in[index1];
    in[index1] = in[index2];
    in[index2] = tmp;
  }
}

//! cree un tableau de nbCards aléatoire
/*!
  \param[in] nbCards nombre de cartes de notre packet
  \param[in] nbRandom nombre passe du random
  \return pointeur sur la memoire du packet
*/
char *createPacket(size_t nbCards, size_t nbRandom){
  char *ret = malloc(sizeof(char)*nbCards);
  if(ret == NULL){
    fprintf(stderr,"%s\n",strerror(errno));
    exit(errno);
  }
  for(size_t i=0; i<nbCards;i++){
    ret[i]=i+1;
  }
  packetFlush(ret,nbCards,nbRandom);

  return ret;
}

//! distribution des cartes a un seule joueur
/*!
  \param[out] cli pointeur sur la structure client
  \param[in] pointeur sur le packet
  \param[in] len taille de la main du joueur
*/
void clientGetCards(client *cli, char *packet, size_t len){
  if(cli->cartes!=NULL) free(cli->cartes);
  cli->cartes = malloc(sizeof(char)*len);
  if(cli->cartes == NULL){
    fprintf(stderr,"%s\n",strerror(errno));
    exit(errno);
  }
  fprintf(cli->file_ptr,"\e[1;1H\e[2JVos cartes: ");
  for (size_t i = 0; i < len; i++) {
    cli->cartes[i] = packet[i];
    //envois parte aux client
    fprintf(cli->file_ptr, "%d,",cli->cartes[i]);
  }
  cli->size = len;
  return;
}

//! distribution des cartes a tout les joueurs
/*!
  \param[out] in clientArray pointeur sur le tableau de client
  \param[in] nb_by_client nombre de cartes par client
  \param[in] packet pointeur sur le packet de la partie
*/
void sendCards(clientArray *in,size_t nb_by_client, char *packet){
  for(size_t i=0;i<in->size;i++){
    clientGetCards(&in->lst[i], packet +(i*nb_by_client), nb_by_client);
  }
  return;
}
