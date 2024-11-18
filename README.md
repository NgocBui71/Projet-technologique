# mosaic-a09

## Projet
Mosaic est un jeu de puzzle utilisant la logique.

## Auteurs
Ali Arda Barut | <ali-arda.barut@etu.u-bordeaux.fr>\
Heliarisoa Lucile Rakotondrabe | <heliarisoa-lucile.rakotondrabe@etu.u-bordeaux.fr>\
Phuoc Bao Ngoc Bui | <phuoc-bao-ngoc.bui@etu.u-bordeaux.fr>

## Installation et compilation
1- Il faut vous assurer d'avoir installé CMake et Git sur votre ordinateur.

2- Cloner le projet :
    $ git clone git@gitlab.emi.u-bordeaux.fr:pt2/teams/mosaic-a09.git
    
3- Puis entrer dans le répertoire du projet :
    $ cd mosaic-a09

4- Créer un répertoire pour la compilation et accéder à ce répertoire :
    $mkdir build
    $ cd build

5- Générer les fichiers de constructions avec CMake puis compiler le projet :
    $ cmake ..
    $ make

6- Pour effectuer les tests :
    $ make test

7- Pour détecter les fuites mémoires :
    $ make ExperimentalMemCheck

8- Pour vérifier la couverture de code :
    $ make ExperimentalCoverage

## Utilisation
Afin de jouer au jeu, il faut executer l'executable généré dans le répertoire build :
    $ ./game_text

##  mosaic_A09 Demo with SDL

###  Dependencies
$ apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev (for Linux Debian/Ubuntu)
$ brew install sdl2 sdl2_image sdl2_ttf                     (for MacOS)

###  Compilation
With CMake:

$ mkdir build ; cd build ; cmake .. ; make

###  Run game_sdl

$ ./game_sdl

Play the colors with mouse : one click for 'WHITE', two for 'BLACK' and three for 'EMPTY'.
You can also restart the game, undo or redo a move, solve the game and have the number of solutions.
Enjoy the game!