# Gestion mémoire

## Analyse de la gestion des avions

La création des avions est aujourd'hui gérée par les fonctions `TowerSimulation::create_aircraft` et `TowerSimulation::create_random_aircraft`.
Chaque avion créé est ensuite placé dans les files `GL::display_queue` et `GL::move_queue`.

Imaginez et décrivez ce que vous devriez faire si vous souhaitiez accéder à l'avion ayant le numéro de vol "AF1250".

---

## Objectif 1 - Référencement des avions

### A - Choisir l'architecture

Pour trouver un avion particulier dans le programme, ce serait pratique d'avoir une classe qui référence tous les avions et qui peut donc nous renvoyer celui qui nous intéresse.

Vous avez 2 choix possibles :
- créer une nouvelle classe, `AircraftManager`, qui assumera ce rôle,
- donner ce rôle à une classe existante.

Réfléchissez aux pour et contre de chacune de ces options.

Pour le restant de l'exercice, vous partirez sur le premier choix.

### B - Déterminer le propriétaire de chaque avion

Vous allez introduire une nouvelle liste de références sur les avions du programme.
Il serait donc bon de savoir qui est censé détruire les avions du programme, afin de déterminer comment vous allez pouvoir mettre à jour votre gestionnaire d'avions lorsque l'un d'entre eux disparaît.

Répondez aux questions suivantes :
1. Qui est responsable de détruire les avions du programme ? (si vous ne trouvez pas, faites/continuez la question 4 dans TASK_0)
2. Quelles sont les listes qui contiennent une référence sur un avion au moment où il doit être détruit ?
3. Comment fait-on pour supprimer la référence sur un avion qui va être détruit dans ces deux structures ?

Pour simplifier le programme, l'`AircraftManager` aura l'ownership des avions, c'est-à-dire que c'est lui qui s'occupera de les faire disparaître du programme, et non plus la fonction `timer`. Il aura également la responsabilité de les faire bouger.

### C - C'est parti !

Ajoutez un attribut `aircrafts` dans le gestionnaire d'avions.
Choisissez un type qui met bien en avant le fait que `AircraftManager` est propriétaire des avions, et vérifiez avec votre chargé de TP qu'il s'agit de la bonne solution.

Ajoutez un nouvel attribut `aircraft_manager` dans la classe `TowerSimulation`.

La fonction `timer` est implémentée dans la partie GL/ du programme. Celle-ci est complètement indépendente du contenu de l'application, c'est-à-dire qu'elle ne connaît pas les types `Aircraft`, `Airport`, etc.\
Pour conserver cette indépendence, et que `timer` puisse tout de même demander à l'`AircraftManager` de déplacer les avions, `AircraftManager` devra hériter de `DynamicObject` et être inséré dans la `move_queue`.\
Implémentez ensuite `AircraftManager::move` afin que qu'il déplace les avions et supprime ceux qui doivent sortir du programme.\
N'oubliez pas de retirer les ajouts d'avions à la `move_queue`, sinon, ceux-ci seront déplacés et supprimés 2 fois (et par l'`AircraftManager`, et par `timer`).\
Vous pouvez maintenant supprimer la relation d'héritage entre `Aircraft` et `DynamicObject`, puisse que le seul intérêt à être un `DynamicObject`, c'est de pouvoir être placé dans la `move_queue` pour être mis à jour par `timer`.

Enfin, faites ce qu'il faut pour que `create_aircraft` donne l'avion qu'elle crée au gestionnaire.
Testez que le programme fonctionne toujours.

---

## Objectif 2 - Usine à avions

### A - Création d'une factory

La création des avions est faite à partir des composants suivants :
- `create_aircraft`
- `create_random_aircraft`
- `airlines`
- `aircraft_types`.

Pour éviter l'usage de variables globales, vous allez créer une classe `AircraftFactory` dont le rôle est de créer des avions.

Définissez cette classe, instanciez-là en tant que membre de `TowerSimulation` et refactorisez-le code pour l'utiliser.
Vous devriez constater que le programme crashe.

En effet, pour que la factory fonctionne, il faut que le `MediaPath` (avec la fonction `MediaPath::initialize`) et que `glut` (avec la fonction `init_gl()`) aient été initialisés.
Comme ces appels sont faits depuis le corps du constructeur de `TowerSimulation`, ils sont actuellement exécutés après la construction de la factory.
Afin de faire en sorte que les appels aient lieu dans le bon ordre, vous allez créer une structure `ContextInitializer` dans le fichier `tower_sim.hpp`.
Vous lui ajouterez un constructeur dont le rôle sera d'appeler les fonctions d'initialisation de `MediaPath`, `glut` et `srand`.

Vous pouvez maintenant ajoutez un attribut `context_initializer` de type `ContextInitializer` dans la classe `TowerSimulation`.
A quelle ligne faut-il définir `context_initializer` dans `TowerSimulation` pour s'assurer que le constructeur de `context_initializer` est appelé avant celui de `factory` ?

Refactorisez le restant du code pour utiliser votre factory.
Vous devriez du coup pouvoir supprimer les variables globales `airlines` et `aircraft_types`.

Essayez de supprimer au maximum les pointeurs nus, et de les remplacer par des types qui permettent d'exprimer clairement l'ownership. N'hésitez pas à demander des conseils à votre chargé de TP ou à vos camarades.

### B - Conflits

Il est rare, mais possible, que deux avions soient créés avec le même numéro de vol.
Ajoutez un conteneur dans votre classe `AircraftFactory` contenant tous les numéros de vol déjà utilisés.
Faites maintenant en sorte qu'il ne soit plus possible de créer deux fois un avion avec le même numéro de vol.

### C - Data-driven AircraftType (optionnel)

On aimerait pouvoir charger les paramètres des avions depuis un fichier.

Définissez un format de fichier qui contiendrait les informations de chaque `AircraftType` disponible dans le programme.\
Ajoutez une fonction `AircraftFactory::LoadTypes(const MediaPath&)` permettant de charger ce fichier.
Les anciens `AircraftTypes` sont supprimés.

Modifiez ensuite le `main`, afin de permettre à l'utilisateur de passer le chemin de ce fichier via les paramètres du programme.
S'il ne le fait pas, on utilise la liste de type par défaut.

Si vous voulez de nouveaux sprites, vous pouvez en trouver sur [cette page](http://www.as-st.com/ttd/planes/planes.html)
(un peu de retouche par GIMP est necessaire)

---

## Objectif 3 - Pool de textures (optionnel)

Pour le moment, chacun des `AircraftType` contient et charge ses propres sprites.
On pourrait néanmoins avoir différents `AircraftType` qui utilisent les mêmes sprites.
Ils seraient donc chargés plusieurs fois depuis le disque pour rien.

Pour rendre le programme un peu plus performant, implémentez une classe `TexturePool` qui s'occupe de charger, stocker et fournir les textures.
Pour exprimer correctement ce type d'ownership, vous devez utiliser le smart-pointer `std::shared_ptr`.

Commencez par aller sur la documentation de `std::shared_ptr`.
Pouvez-vous expliquer comment le compilateur arrive à déterminer à quel moment l'objet n'a plus aucun owner, afin de le détruire ?

Listez les classes qui ont besoin de `TexturePool`.
Sachant que vous n'aurez qu'une seule instance de `TexturePool` dans votre programme, quelle classe devra assumer l'ownership de cet objet ?\
Instanciez `TexturePool` au bon endroit et refactorisez le code afin que tous les chargements de textures utilisent ce nouvel objet.
Assurez-vous que votre implémentation ne génère pas des fuites de mémoire au moment de sa destruction.
