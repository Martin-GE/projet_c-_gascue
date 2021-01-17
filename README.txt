DESCRIPTION DU PROGRAMME

Au long de votre lecture du code vous trouverez des explications détaillant la démarche suivie 
pour coder les différentes fonctions. Ce document a pour but de prendre du recul et comprendre 
comment est constitué l'ensemble du programme

Le programme est divisé en deux parties :

    La partie permettant de créer un fichier .wav contenant un message en morse à partir d'un 
    texte introduit manuellement par l'utilisateur ou contenu dans un fichier .txt dont 
    l'utilisateur fournit le nom. Les fonctions impliquées sont codées dans le fichier 
    codage.cpp et déclarées dans le fichier codage.h

    La partie permettant de traduire le message en morse contenu dans un fichier .wav dont le 
    nom est fourni par l'utilisateur. Ce message est ensuite enregistré dans un fichier créé 
    automatiquement par le programme. Les fonctions impliquées sont codées dans le fichier 
    decodage.cpp et déclarées dans le fichier decodage.h

Le fichier test.cpp contient la fonction main() et exécute le codage ou le décodage selon ce 
que l'utilisateur veut faire

Lors de la compilation, il faut que tous les fichiers soient dans le même dossier (y compris le 
.txt ou .wav qui contiennent les messages qui doivent être traduits)

Pour compiler il suffit d'écrire "make" dans le terminal. Lorsque les fichiers sont compilés, 
il faut appeler l'exécutable "./programme"

Nous avons choisi comme convention de durée de temps pour les messages en morse :
0.2 secondes pour le bip court dure 0.2 secondes 
3*(bip court) = 0.6 secondes pour le bip long 
0.2 secondes pour le silence entre les bips 
3*(bip court) = 0.6 secondes pour les silences entre les lettres 
7*(bip court) = 1.4 secondes pour les silences entre les paroles 

Si une convention différente est utilisée, les messages ne peuvent pas être décodés du morse 
vers les lettres (il suffit de modifier les parties du code concernées, ce qui est très vite 
reglé lorsque nous connaissons les conventions de temps du message reçu)