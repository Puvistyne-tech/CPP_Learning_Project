# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
Sur quelle touche faut-il appuyer pour ajouter un avion ?
>c 
 
Comment faire pour quitter le programme ?
> q ou x

A quoi sert la touche 'F' ?
> Toggle fullscreen mode


Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?
>A plane can fly, Land to a Terminal for servicing and it can take off 
> 
Quelles informations s'affichent dans la console ?
> *  Aircraft Landing and Take off information
> * Terminal servicing information (start/stop)
> 
Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ?
> Since the airport has only three terminals, only three planes can be served by the 
> airport simultaneously. The fourth aircraft had to wait for a free terminal

## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.
>
> Aircraft : C'est un avion  
> Airport : C'est l'aéroport  
> AirportType : Fournit la position des éléments de l'aéroport  
> Terminal : Prend un aircraft et s'occupe de lui pendant un certain temps  
> Tower : Donne les instructions aux avions.  
> TowerSimulation : Gère les actions utilisateur  
> Waypoint : Représente un point en 3D ayant une connaissance de s'il est au sol ou non
> 
Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.

> Tower :
> * get_instructions(Aircraft&) : Génère des instructions pour l'avion
> * arrived_at_terminal(Aircraft&) : Récupère le terminal sur lequel doit se poser l'avions et ensuite lui donne l'avion
>
> Aircraft :
> * get_flight_num() : Renvoi le numéro de vol de cet avion
> * distance_to(): Renvoi la distance entre cet avion et un point
> * display() : Affiche l'avion
> * move() : Met à jour la position de l'avion
>
> Airport :
> * get_tower() : Renvoi la tour de control
> * display() : Affiche l'aéroport
> * move() : fait bouger tous les terminaux
>
> Terminal :
> * in_use() : Renvoi true s'il est en cours d'utilisation
> * is_servicing() : Renvoi true tant qu'il n'a pas fini de s'occuper de l'avion
> * assign_craft() : Assigne un nouvel avions
> * start_service() : Affiche un message et commence à s'occuper de l'avion
> * finish_service() : Affiche un message et supprime sa ref sur l'avion
> * move() : Ajoute 1 au compteur de traitement de l'avion

Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
> On commence dans Aircraft::move() qui appel
> Tower::get_instruction() si il n'a plus de waypoints
> Ensuite appel essaie de reserver un terminal avec Airport::reserve_terminal()
> qui appel Terminal::assign_craft() si c'est réussi et met à jour le chemin de l'avion  
> sinon assigne un nouveau chemin aérien à l'avion
> Dans le cas où l'avion est dans un terminal si l'avion a fini d'etre servit,
> retire l'avion/terminal des reservations et fait repartir l'avion
> 
Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
Expliquez les intérêts de ce choix.
> une queue parce qu'on veut ajouter des points à la fin et pouvoir les récupérer à partir du début


## C- Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.

> AircraftTypes: changer le 2ème paramètre(``max_air_speed``) de à .2f


2) Identifiez quelle variable contrôle le framerate de la simulation.\
Le framerate correspond au temps de rafraichissement du programme, c'est-à-dire le nombre de fois où les éléments du programme seront mis à jour (ajout de nouvel avion à la simulation, déplacement, etc) en une seconde.\
Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\
Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.

> **GL::tick_per_frame** initialise avec **config::DEFAULT_TICKS_PER_SEC**.
> ajout de 2 inputs : **i** pour incrémenter de 1 le framerate et **d** pour décrémenter de 1 le framerate.
> Si tick_per_sec passe à 0, dans la fonction *``GL::timer``* il y a une division par **0**  
> Pour ajouter la pause, j'ajoute encore un input **p** permettant de passer entre le framerate = 0 et
> celui qu'il y avait avant la pause (ajout d'une variable statique)

3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.
> config::SERVICE_CYCLES -> 40u

4) Lorsqu'un avion a décollé, il réatterrit peu de temps après.
Assurez-vous qu'à la place, il soit supprimé de la `move_queue`.\
Pour tester, il suffit de dézoomer et de vérifier que les avions suffisamment éloignés ne bougent plus.
Indices :\
A quel endroit pouvez-vous savoir que l'avion doit être supprimé ? ``> Aircraft::move``\
Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ? ``> parce que le seul qui own les avions est GL::move_queue``
A quel endroit de la callstack pourriez-vous le faire à la place ?\ ``> GL::timer``
Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ? ```un boolean```

6) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.\
Essayez maintenant de supprimer complètement l'avion du programme lorsque vous le retirez de la `move_queue`.\
En dézoomant, vous devriez maintenant constater que les avions disparaissent maintenant de l'écran.
> on met l'ajout et la suppression des elements de la display queue dans le constructeur et le destructeur de Displayable.
> On peut donc retirer les ajouts et les suppressions


7) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.
> Utiliser une unordered_map
> 

## D- Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?
> On a fait en sorte que seul Airport appel assign_craft
> 

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference constante ?
Pourquoi n'est-il pas possible d'éviter la copie du `Point3D` passé en paramètre ?
> Parce qu'on veut appliquer un traitement à `direction` avant de l'ajouter à la vitesse.
> C'est possible, mais pour ça il faudrait faire les traitements différamment.

## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.

> Vu qu'on a modifié la class Displayable, C'est lui qui occupé la création et la destruction des displayables, 
> Donc, on n'a plus besoin de l'appeler (ajouter et retirer) dans la class Tower_sim.cpp lors de la création 
> d'aéroport et des aircraft.