#include "serveur.h"

//! Crée un serveur et renvois le filedescriptor
int createSock(){
  int sock = socket(AF_INET, SOCK_STREAM,0);
  if(sock == -1){
    fprintf(stderr, "%s\n", strerror(errno));
    exit(errno);
  }
  struct sockaddr_in sin;
  socklen_t recsize = sizeof(sin);
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(PORT);
  if (bind(sock, (struct sockaddr*)&sin, recsize) == -1) {
      fprintf(stderr,"%s\n", strerror(errno));
      exit(errno);
  }
  if (listen(sock, 5) == -1) {
      fprintf(stderr,"%s\n", strerror(errno));
      exit(errno);
  }
  return sock;
}

//! thread initialisation client
/*!
  \param ptrClient pointeur sur la strucutre client
  \return NULL defeat warning
*/
void *pthreadInitClient(void *ptrClient){
  client *cli = (client *)ptrClient;
  fprintf(cli->file_ptr, "Entrez votre nom : ");
  fgets(cli->name,SIZE_NAME,cli->file_ptr);
  cli->name[strlen(cli->name)-1] = '\0';
  cli->size=0;
  cli->cartes=NULL;
  pthread_exit (ptrClient);
}


//! thread demande valeur y ou n aux client
/*!
  \param ptrClient pointeur sur la strucutre client
  \return return pointeur sur le char
*/
void *pthreadAskClient(void *ptrClient){
  char *reponse = malloc(sizeof(char)*3);
  memset(reponse,0,3);
  client *cli=(client *)ptrClient;
  if(reponse==NULL) FATAL();
  do {
    fprintf(cli->file_ptr,"\e[1;1H\e[2JVoulez vous rejouer une partie ?(y/n): ");
    fgets(reponse,3,cli->file_ptr);
  } while(reponse[0]!='y' && reponse[0]!='n');
  pthread_exit((void *)reponse);
}

//! Accept connexion
/*!
  \param[in] serverSock serveur
  \param[out] clientArr pointeur sur la structure clientArray
  \param[in] nb nombre de client
*/
void acceptClient(int serverSock,clientArray *clientArr, size_t nb){
  struct sockaddr_in sin;
  socklen_t csize = sizeof(sin);
  int tmp;
  int retpthread;
  char *ip;
  pthread_t *pthread_t_lst = malloc(sizeof(pthread_t)*nb);
  if(pthread_t_lst == NULL)FATAL();
  for(size_t i=0; i<nb;i++){
    tmp = accept(serverSock, (struct sockaddr *)&sin, &csize);
    if(tmp==-1)FATAL();
    ip = inet_ntoa(sin.sin_addr);
    printf("Incoming connection [%s]\n",ip);
    clientArr->lst[i].file_ptr = fdopen(tmp,"a+");
    if(clientArr->lst[i].file_ptr == NULL)FATAL();

    setvbuf(clientArr->lst[i].file_ptr,NULL, _IONBF, 0);
    retpthread = pthread_create(pthread_t_lst +i, NULL, pthreadInitClient, (void *)(clientArr->lst + i));
    if(retpthread!=0)FATAL();
  }
  //wait all thread
  for(size_t i=0; i < nb; i++){
    retpthread = pthread_join(pthread_t_lst[i],NULL);
    if(retpthread!=0)FATAL();
  }
  free(pthread_t_lst);
  return;
}

//! Crée la structure clientArray dans le heap
/*!
  \param[in] max_client nombre max de clients
  \return pointeur sur la structure
*/
clientArray* createClientArray(size_t max_client){
  clientArray* ret = malloc(sizeof(clientArray));
  if(ret==NULL)FATAL();
  ret->lst = malloc(sizeof(client)*max_client);
  if(ret->lst == NULL)FATAL();
  for(size_t i=0; i < max_client; i++){
    ret->lst[i].file_ptr=NULL;
    ret->lst[i].cartes = NULL;

    ret->lst[i].prevTime = time(NULL);
    ret->lst[i].temp = 0;
    ret->lst[i].nbFails = 0;
    ret->lst[i].nbCoupJoue = 0;
  }
  ret->size = max_client;
  return ret;
}

//! Déallocation de la mémoire pour clientArray
void freeClientArray(clientArray *in){
  for(size_t i=0; i<in->size ; i++){
    fclose(in->lst[i].file_ptr);
    if(in->lst[i].cartes!=NULL){
      free(in->lst[i].cartes);
    }
  }
  free(in->lst);
  free(in);
  return;
}

//! Demande aux joeurs pour une potentiel nouvelle partie
bool checkNewGame(clientArray *in){
    pthread_t *lst = malloc(sizeof(pthread_t)*in->size);
    int check;
    if(lst==NULL)FATAL();
    for(size_t i=0;i<in->size;i++){
      check=pthread_create(lst+i,NULL, pthreadAskClient, (void *)(in->lst+i));

      if(check!=0)FATAL();
    }
    char *ret;
    char bool_ret='y';
    for(size_t i=0; i<in->size;i++){
      pthread_join(lst[i], (void *)&ret);
      bool_ret = bool_ret & (*ret);
      free(ret);
    }
    free(lst);
    if(bool_ret=='y') return true;
    return false;
}
