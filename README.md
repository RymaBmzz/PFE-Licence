# PFE-Licence
jeu interactif homme machine basé sur la reconnaissance en temps réel des gestes de la main. 
La thématique de notre projet de fin d’études consiste à créer un jeu interactif homme machine
basé sur la reconnaissance en temps réel des gestes de la main. 
On utilise une kinect pour l’acquisition du flux vidéo.
Le projet est défini par plusieurs phases et méthodes. Nous débutons avec la segmentation
du flux acquis pour détecter les pixels peau puis nous supprimons le visage pour ne garder
que la main. Par la suite nous cherchons le contour afin d’en extraire les caractéristiques (dont
les pics et les vallées) qui nous permettront de définir la courbe de jonction qui sépare la paume
des doigts en utilisant la méthode des moindres carrées, ainsi que l’identification du geste dans la
partie reconnaissance.
Nous avons réalisé un jeu interactif "Puzzle" qui se base sur la reconnaissance des gestes
de la main et un descripteur global de la posture effectuée.
Les résultats obtenus pendant la phase des tests sont encourageants.
Mots clé : segmentation, contour, courbe de jonction,reconnaissance, jeu interactif, descripteur
global.
