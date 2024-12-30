## Presentation

Ce projet est une implémentation du [Mandelbrot Set](https://fr.wikipedia.org/wiki/Ensemble_de_Mandelbrot) en C++ avec la bibliothèque [SDL2](https://www.libsdl.org/).
On peut zoomer sur l'ensemble de Mandelbrot en cliquant et en selectionnant une zone de l'écran avec la souris.

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