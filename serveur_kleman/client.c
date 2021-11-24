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

//! fonction d'affichage de la main d'un client
/*!
  \param[in] cli pointeur sur client
  \param[in] pointeur sur la sortie utilisé
*/
void clientPrint(client *cli, FILE *out){
  fprintf(out,"Joueur %s :",cli->name);
  for(size_t i=0;i<cli->size;i++){
    fprintf(out,"%d(%lu),",cli->cartes[i],i);
  }
  fprintf(out, "\n");
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
  if(ret == NULL)FATAL();
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
  if(cli->cartes == NULL)FATAL();
  for (size_t i = 0; i < len; i++) {
    cli->cartes[i] = packet[i];
  }
  cli->size = len;
  fprintf(cli->file_ptr, "\nEntrez l'index de la carte que vous voulez jouer\n");
  clientPrint(cli, cli->file_ptr);
  return;
}

//! supprime une carte d'un client
/*!
  \param[out] in pointeur sur le client
  \param[in] index de la carte a supprimer
*/
void clientDelCard(client *in, size_t index){
  if(in->size==0) return;
  size_t i=0;
  size_t i1=0;
  char *new = malloc(sizeof(char)*(in->size-1));
  if(new==NULL) FATAL();
  while (i<(in->size-1)) {
    if(i1==index) i1++;
    new[i] = in->cartes[i1];
    i++;
    i1++;
  }
  free(in->cartes);
  in->cartes = new;
  in->size--;
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

//! renvois true si tout les joueurs on défaussé toute leurs cartes
/*!
  \param[in] in pointeur sur le la list de client
  \return false si tout les client on 0 cartes
*/
bool checkAllClientEmpty(clientArray *in){
  for (size_t i = 0; i < in->size; i++) {
    if(in->lst[i].size != 0) return true;
  }
  return false;
}
//! change le mode d'input en non bloquand pour tout les joueurs
/*!
  \param[in] in pointeur sur clientArray
*/
void changeAllClientIOBlock(clientArray *in){
  int status;
  int fd;
  for(size_t i=0;i<in->size;i++){
    fd = fileno(in->lst[i].file_ptr);
    status = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) & (~O_NONBLOCK));
    if(status==-1) FATAL();
  }
}

//A DEBATRE
//!envois a tout les clients une chaine
/*!
  \param[in] in pointeur sur la list de client
  \param[in] str pointeur sur une chaine de char
*/
// void sendAllClient(clientArray *in, char *str){
//   for(size_t i=0;i<in->size;i++){
//     fprintf(stderr, "%s", str);
//   }
// }

//! change le mode d'input en bloquand pour tout les joueurs
/*!
  \param[in] in pointeur sur clientArray
*/
void changeAllClientIONonBlock(clientArray *in){
  int status;
  int fd;
  for(size_t i=0;i<in->size;i++){
    fd = fileno(in->lst[i].file_ptr);
    status = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
    if(status==-1) FATAL();
  }
}
