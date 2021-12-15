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
      <li><a href="#client">Client</a></li>
      <li><a href="#bot">Bot</a></li>
  </ul>
  <li><a href="#setup">Configuration du serveur</a></li>
    <ul>
      <li><a href="#apache">apache</a></li>
      <li><a href="#docker">docker</a></li>
  </ul>
</ol>


### Présentation {#presentation}

L'objectif est de créer le jeu the mid avec les outils vus en cours de système et réseaux.<br/>
Le jeux consiste a distribuer des cartes de 1 a 100 aux joueurs.
A la manche 1, les joueurs ont 1 carte, à la manche 2 deux cartes et ainsi de suite.</br>
En cas d'échec, les joueurs recommencent à la manche 1. Le but est que tous les joueurs posent leurs cartes dans l'odre croissant sans ordre de passage ni communication.
Toute la subtilité du projet tourne autour de la mécanique qui permet aux joueurs de jouer quand ils veulent.<br/>
Toutes les sources de ce projet sont disponible sur https://github.com/requin-citron/projet_system



### Architecture {#architecture}

#### Client Serveur {#client_serveur}

Nous avons décidé d'utiliser une architecture client serveur pour gérer ce problème. L'avantage des sockets sur IPC est que nous pouvons avoir nos joueurs sur des machines différentes.<br/>
Nous n'avons pas développé de client car nous préférons utiliser un client normalisé comme [netcat](https://fr.wikipedia.org/wiki/Netcat).

#### Serveur {#serveur}

Le programme serveur permet d'écouter sur toutes les interfaces
sur un port hardcodé dans un fichier header.<br/>
Il faut spécifier deux paramètres aux binaires : le nombre de joueurs ainsi que le nombre de manche pour gagner.
Toute la gestion se fait coté serveur ; le client ne sert qu'à afficher les informations envoyées par le serveur ainsi qu'à envoyer les entrées utilisateur.Le code du serveur se décompose en trois fichiers: le fichier client.c qui gère tout ce qui concerne les clients, la gestion de leurs input output ainsi que la gestion des cartes, le fichier serveur.c possède toutes les primitives pour le serveur création du socket gestion des threads et le fichier main.c qui possède tout le code propre aux jeu.

#### Génération du PDF {#pdfgen}

pour la génération du pdf nous utilisons la commande pdflatex avec une template en latex.<br/>
pour envoyer le document aux clients nous envoyons un lien de téléchargement.


#### Serveur Web et Sécurité {#configserveur}

Pour l'envoi des pdfs aux clients par un lien internet, il est nessaisaire de créer un site web.Pour cela nous allons expliquer le fonctionnement.<br/>
Nous utilisons mon VPS personnelle pour héberger le serveur web.
Pour notre service, il nous faut 3 éléments : un serveur web, un certificat pour le https ainsi qu'un site qui gère l'upload.<br/>
Le VPS possède déjà plusieurs sites web utilisant apache.
Pour le site, un projet open source et trés connu en go existe transfer.sh https://github.com/dutchcoders/transfer.sh.<br/>
Ce projet est trés pratique vu qu'il permet d'upload trés simplement grâce à curl nos fichiers.Le certificat let's encrypt que j'ai pour mon domaine tukif.info est un dns challenge donc tous les sous domaines profitent du certificat.De plus, nous évitons les attaques d'identification de sous nom de domaine https://crt.sh/.<br/>
De plus nous utilisons docker pour isoler le service des autres.Nous avons donc créé un service systemd qui démarre un transfer.sh dans un docker en local montant un répertoire commun. Par default docker ne sandbox pas les id des users. Nous faisons donc tourner le docker avec un user qui a le même uid que l'utilisateur www-data du master. Il ne reste plus qu'à utiliser le system de VHOST d'apache et le configurer en mode proxy(de plus, il y a un system pour les headers mais ce ne sera pas abordé).

### Implementation {#implementation}

#### Utilisation Linux {#linux}

Dans linux, tout est fichier ce qui implique que nous pouvons considérer que les sockets sont des fichiers.Nous allons donc nous servir de ça dans le serveur en transformant tous les file descriptors des clients en FILE * pour pouvoir nous servir des fonctions fgets et fprintf de la libc.Il ne reste plus qu'à désactiver le IO buffering pour avoir un résultat en temps réel.<br/>
Nous voulions créer un bot en bash. Pour cela nous avons utilisé une autre particularité de Linux. Linux utilise trois flux par processus stdin stdout et stderr représenté par les files descriptors 0 1 2. Nous pouvons remplacer stdin et stdout par le file descriptor du socket et utiliser exec pour remplacer le processus par le script en bash ce qui permet d'utiliser read et echo pour communiquer avec le socket.Il reste la sortie d'erreur pour afficher des informations à l'écran.

#### Serveur {#serveur_des}

Nous utilisons des thread pour accepter tous les clients pour cela nous utilisons pthread.Une fois tous les clients acceptés, le programme rentre dans une grande boucle qui permet de faire le jeu. L'astuce utilisée est de passer tous les files descriptors des clients en mode non bloquant(CF [man fcntl](http://manpagesfr.free.fr/man/man2/fcntl.2.html)).<br/>
Cette astuce permet aux clients de jouer quand ils veulent.Nous avons fait une fonction permettant de faire l'inverse pour la proposition de refaire une partie en mode bloquant.
Nous utilisons curl pour envoyer le fichier sur le serveur.L'astuce est d'utiliser la commande curl qui posséde un argument pour créer un fichier C qui fait la même chose que la commande.


#### PDF {#pdfimp}

Nous avons fait une template en latex avec un \%s dedans ce qui permet d'injecter du latex dans le fichier. Nous utilisons ensuite la fonction system pour appeller pdflatex. Nous avons désactivé les write8 pour éviter toute injection de commandes dans le latex, par contre notre solution ne gére pas les pseudos avec des caractéres non suportés par latex.
De plus la fonction system n'est pas appellé avec des inputs user nous n'avons donc pas d'injection de commande.

#### Client {#client}

Nous avons crée un client dans le cas où vous ne vouliez pas utiliser de client normalisé (cf [netcat](https://fr.wikipedia.org/wiki/Netcat)).
Le client possède une ip hardcodé dans dans le fichier ainsi qu'un port hardcodé.Nous utilisons un thread de réception qui affiche en continu ce que le serveur envoie.Dans la fonction principale, nous avons envoyé tout ce que l'utilisateur tape au serveur ce qui permet d'avoir un comportement similaire a [netcat](https://fr.wikipedia.org/wiki/Netcat).

#### Bot {#bot}

Nous avons crée un bot en bash.Il se décompose en deux parties: une partie qui gère le réseau en c ainsi qu'une partie qui gère la partie en bash. Nous utilisons donc un wrapper qui permet d'ouvrir un socket puis qui remplace le filedescriptor de stdin et stdout par celui du socket.Maintenant tout ce qui est printé va dans le socket et tout ce qui est lu vient du socket.Le wrapper fait ensuite un execvp sur un script en bash pour conserver l'environnement.
Par default les exec conservent les filedescriptor ce qui permet de pouvoir dans le script en bash intéragir avec le socket via les echo et les read.Pour réaliser cela nous avons utilisé la fonction [dup2](http://manpagesfr.free.fr/man/man2/dup.2.html).Le code du bot est simple il récupére la liste des cartes, la trie dans l'ordre croissant puis les joue une par une en attendant un petit peu à chaque carte.Le bot va dormir 0.1 seconde fois la valeur de la carte puis en faisant de même avec la carte d'après en soustrayant la valeur déjà attendue.

### Configuration du serveur {#setup}

Nous allons maintenant détailler la configuration du serveur.Ce n'etait pas demandé dans le sujet mais étant donné que nous sommes en cours de system nous avons décidé de le décrire sommairement ici.

#### Apache {#apache}

  [Apache](https://httpd.apache.org/) est un serveur web écrit en c.[Apache](https://httpd.apache.org/) est le webserver le plus connu bien qu'il perd de plus en plus de terrain face à nginx.
  Apache possède un system de [virtual host](https://httpd.apache.org/docs/2.4/fr/vhosts/examples.html) qui permet de renvoyer différentes pages selon le nom de domaine utilisé pour se connecter (cf [header host](https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Host))

  ![](../images/apache_config.png)

  Nous voyons sur la config que nous renvoyons les request sur le localhost sur le port 9658.
  Le reste concerne le https. On autorise que les algorithmes considérés comme [fiable](https://www.thesslstore.com/blog/tls-1-3-everything-possibly-needed-know/).


#### Docker {#docker}

  [Docker](https://www.docker.com/) est une technologie de containerisation trés utilisée qui permet d'isoler ses services permettant une plus grande portabilité ainsi qu'une plus grande sécurité.Pour le projet, nous avions deux contraintes : pouvoir récupérer les fichiers même si l'on redémarre le docker ainsi que pouvoir logger tout ce qu'il se passe.Nous avons donc crée un service [systemd](https://fr.wikipedia.org/wiki/Systemd) permettant de faire tout ça.

  ```dockerfile
  FROM alpine:latest

  RUN apk add --no-cache libc6-compat shadow
  COPY ./transfer.sh /transfer.sh
  RUN mkdir /data
  VOLUME ["/data/"]
  EXPOSE 8080


  USER xfs
  CMD ["/transfer.sh", "--provider", "local", "--basedir", "/data", "--listener", "0.0.0.0:8080"]
  ```

  Ci dessus le [Docker](https://www.docker.com/) file permettant de créer le [conteneur](https://fr.wikipedia.org/wiki/Conteneur_(informatique)) le fichier [transfer.sh](https://github.com/dutchcoders/transfer.sh) et le binaire qui gérera l'upload.
  De plus, nous utilisons l'utilisateur xfs qui a le même uid que www-data du master qui permet d'avoir les fichiers écrits sur le fs avec les mêmes droits que le serveur apache. Ce qui évite, en cas de compromition, qu'un attaquant crée un wrapper suid en root.
  Exemple du dégat que cela peu engendrer: https://podalirius.net/fr/writeups/heroctf-2021-rooter-l-infra-for-fun-and-ctf-points/

```
[Unit]
Description=transfer.sh Container
After=docker.service
Requires=docker.service

[Service]
Type=simple
TimeoutStartSec=0
Restart=always
ExecStart=/usr/bin/docker run --rm --name transfersh -p 9658:8080 -v /transfer.sh/:/data/ transfer.sh
ExecStop=/usr/bin/docker stop transfersh

[Install]
WantedBy=multi-user.target
```

Il ne reste plus qu'à l'inclure dans un service systemd.On spécifie le type de service on avec le restart en cas d'erreur ainsi que la [target](https://www.freedesktop.org/software/systemd/man/systemd.target.html).<br/>
Nous avons maintenant un service qui se relance automatiquement en cas d'erreur, qui log tout et qui stocke les fichiers hors du conteneur.

 ```
/etc/systemd/system ❯ journalctl -e -u docker.transfersh.service -n 10
Dec 07 15:20:12 VPS-QLF docker[16798]: [transfer.sh]2021/12/07 15:20:12 109.12.127.36:0 - - [2021-12-07T15:20:12Z] "GET /images/revie
Dec 07 15:20:12 VPS-QLF docker[16798]: [transfer.sh]2021/12/07 15:20:12 109.12.127.36:0 - - [2021-12-07T15:20:12Z] "GET /images/tor.s
Dec 07 15:20:12 VPS-QLF docker[16798]: [transfer.sh]2021/12/07 15:20:12 109.12.127.36:0 - - [2021-12-07T15:20:12Z] "GET /images/bitco
Dec 07 15:20:12 VPS-QLF docker[16798]: [transfer.sh]2021/12/07 15:20:12 109.12.127.36:0 - - [2021-12-07T15:20:12Z] "GET /images/Logo-
Dec 07 15:20:12 VPS-QLF docker[16798]: [transfer.sh]2021/12/07 15:20:12 109.12.127.36:0 - - [2021-12-07T15:20:12Z] "GET /images/termi
Dec 07 15:20:12 VPS-QLF docker[16798]: [transfer.sh]2021/12/07 15:20:12 109.12.127.36:0 - - [2021-12-07T15:20:12Z] "GET /fonts/transf
Dec 07 19:37:31 VPS-QLF docker[16798]: [transfer.sh]2021/12/07 19:37:31 109.12.127.36:0 - - [2021-12-07T19:37:31Z] "GET /GN3IXo/out.p
Dec 07 19:37:33 VPS-QLF docker[16798]: [transfer.sh]2021/12/07 19:37:33 109.12.127.36:0 - - [2021-12-07T19:37:33Z] "GET /favicon.ico
```
