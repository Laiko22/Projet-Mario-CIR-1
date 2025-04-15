#include "mario.h"

int cameraX = 0; // Position de la caméra

void affichage(char map[H_MAP][L_MAP]) {
    printf("\033[H"); // Curseur en haut

    char ligne[L_SCREEN + 1];
    ligne[L_SCREEN] = '\0';

    for (int i = 0; i < H_SCREEN; i++) {
        for (int j = 0; j < L_SCREEN; j++) {
            ligne[j] = map[i][j + cameraX];
        }
        printf("%s\n", ligne);
    }
}


void commande(int touche, char map[H_MAP][L_MAP]) {
    if (touche == 75) { // GAUCHE
        if (cameraX > 0)
            cameraX--;
    }
    else if (touche == 77) { // DROITE
        if (cameraX < L_MAP - L_SCREEN)
            cameraX++;
    }
    else if (touche == 32) {
        // Gestion du saut (à faire)
    }

    affichage(map);
}