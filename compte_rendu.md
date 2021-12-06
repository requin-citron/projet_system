## Compte Rendu


<!-- TABLE OF CONTENTS -->
<summary>Sommaire</summary>
<ol>
  <li><a href="#presentation">Présentation</a></li>
  <li>
      <a href="#architecture">Architecture</a>
      <ul>
        <li><a href="#client_serveur">Client Serveur</a></li>
        <li><a href="#serveur">serveur</a></li>
        <li><a href="#pdfgen">Génération du PDF</a></li>
        <li><a href="#configserveur">Serveur Web et Sécurité</a></li>
      </ul>
  </li>
  <li><a href="#implementation">Implementation</a></li>
    <ul>
      <li><a href="#linux">Utilisation Linux</a></li>
      <li><a href="#serveur_des">Serveur</a></li>
      <li><a href="#pdfimp">PDF</a></li>
  </ul>
</ol>


### Présentation {#presentation}

L'objectif est de crée le jeu the mid avec les outils vue en cours de système et réseaux.<br/>
Le jeux consiste a distribuer des cartes de 1 a 100 aux joueurs.
A la manche 1 les joueurs ont 1 carte a la manche 2 deux cartes et ainsi de suite.</br>
En cas d'échec les joueurs recommence manche 1. Le but étant que tout les joueurs pose leurs carte dans l'odre croissant sans ordre ordre de passage ni communication.
Toute la subtilité du projet tourne autour de la mécanique qui permet aux joueurs de jouer quand ils veulents.



### Architecture {#architecture}

#### Client Serveur {#client_serveur}

Nous avons décidé d'utiliser une architecture client serveur pour gérer ce problème. L'avantage des sockets sur IPC est que nous pouvons avoir nos joueurs sur des machines différentes.<br/>
Nous n'avons pas développé de client car nous préférons utiliser un client normalisé comme netcat.

#### Serveur {#serveur}

Le programme serveur permet d'écouter sur toutes les interfaces
sur un port hardcodé dans un fichier header.<br/>
Il faut spécifier deux paramétres aux binaire le nombre de joueurs ainsi que le nombre de manche pour gagner.
Toute la gestions ce fait coté serveur le client ne sert qu'a afficher les information envoyés par le serveur ainsi qu'a envoyer les entrées utilisateur.

#### Génération du PDF {#pdfgen}

pour la génération du pdf nous utilisons la commande pdflatex avec une template en latex.<br/>
pour envoyer le document aux clients nous envoyons un lien de téléchargement.


#### Serveur Web et Sécurité {#configserveur}

Pour l'envois des pdf aux client par un lien internet il est nessaisaire de crée un siteweb.Pour cela nous allons expliquer le fonctionnement.<br/>
Nous utilisons mon VPS personnelle pour héberger le serveur web.
Pour notre service il nous faut 3 élément un serveur web un certificat pour le https ainsi qu'un site qui gére l'upload.<br/>
Le VPS possède déjà plusieurs site web utilisant apache.
Pour le site un projet open source et trés connue en go existe transfer.sh https://github.com/dutchcoders/transfer.sh.<br/>
Ce projet est trés pratique vue qu'il permet d'upload trés simplement grace a curl nos fichier.Le certificat let's encrypt que j'ai pour mon domaine tukif.info est un dns challenge donc tout les sous domaines profite du certificat.De plus nous évitons les attaque d'identification de sous nom de domaine https://crt.sh/.<br/>
De plus nous utilisons docker pour isoler de service des autres en cas de compromition du service.Nous avons donc crée un service systemd qui démarre un transfer.sh dans un docker en local montant un répertoire commun. Par default docker ne sandbox pas les id des users nous faison donc tourner le docker avec un user qui a le même uid que l'utilisateur www-data du master. Il ne reste plus qu'a utiliser le system de VHOST d'apache et de le configurer en mode proxy(de plus il y a un system pour les headers mais ce ne sera pas abordé).

### Implementation {#implementation}

#### Utilisation Linux {#linux}

Dans linux tout est fichiers ce qui implique que nous pouvons considérer que les socket sont des fichiers.Nous allons donc nous servir de ca dans le serveur en transformant tout les file descriptors des client en FILE * pour pouvoir nous servir des fonction fgets et fprintf de la libc.Il ne reste plus qu'a desactiver le IO buffering pour avoir un résultat en temps réel.<br/>
Nous voulions crée un bot en bash pour cela nous avons utiliser une autre particularité de linux.Linux utilise trois flux par processus stdin stdout et stderr représenté par les files descriptors 0 1 2 nous pouvons remplacer stdin et stdout par le file descriptor du socket et utiliser exec pour ramplacer le processus par le script en bash ce qui permet d'utiliser read et echo pour communiquer avec le socket.Il reste la sortie d'erreur pour afficher des information a l'écran.

#### Serveur {#serveur_des}

Nous utilisons des thread pour accepté tout les client pour cela nous utilisons pthread.Une fois tout les clients accepté le programme rentre dans une grande boucle qui permet de faire le jeu l'astuce utilisé est de passer tout les files descriptors des clients en mode non bloquant(CF man fcntl).<br/>
Cette astuce permet  aux client de jouer quand ils veulent.

#### PDF {#pdfimp}

Nous avons fais une template en latex avec un %s dedans ce qui permet d'injecter du latex dans le fichier. Nous utilisons ensuite la fonction system pour appeller pdflatex nous avons désactiver les write8 pour éviter toute injection de commande dans le latex, par contre notre solution de gére pas les pseudos avec des caractére non suporté par latex.De plus la fonction system n'est pas appellé avec des inputs user nous n'avons donc pas d'injection de commande.
