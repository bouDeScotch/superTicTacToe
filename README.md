## Presentation

Ce projet est une implémentation d'un jeu dont je n'ai pas retrouvé le nom mais que j'ai appellé "Super Tic-Tac-Toe"
Le jeu ressemble au morpion (tic-tac-toe en anglais), mais chaque case est en fait elle-meme une grille de morpion. La case sur laquelle l'adversaire jouer determine dans quel morpion il faut jouer, sauf si le morpion est deja terminer auquel cas on choisit. Le premier à aligner 3 parties de morpions a gagné, si personne n'y arrive il y a égalité. 

## Installation

### Prérequis

Assurez-vous d'avoir installé les bibliothèques suivantes :

- [SDL2](https://www.libsdl.org/download-2.0.php)

### Instructions

1. Clonez le dépôt :
    ```sh
    git clone https://github.com/bouDeScotch/MandelbrotSetInCpp.git
    cd MandelbrotSetInCpp
    ```

2. Installez les dépendances :
    - **Sur Ubuntu/Debian** :
        ```sh
        sudo apt-get install libsdl2-dev
        ```
    - **Sur macOS** :
        ```sh
        brew install sdl2
        ```
    - **Sur Windows** :
      Téléchargez et installez SDL2 depuis [le site officiel](https://www.libsdl.org/download-2.0.php).

3. Compilez et exécutez le projet :
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ./MandelbrotSetInCpp
    ```