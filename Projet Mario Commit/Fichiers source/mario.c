#include "mario.h"

int cameraX = 0;

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

    affichage(map);
}


int est_solide(int x, int y, char map[H_MAP][L_MAP]) {

    if (x < 0 || x >= L_MAP || y < 0 || y >= H_MAP) {
        return 1; // Considérer les bords comme solides
    }


    char bloc = map[y][x];


    // Définir quels caractères représentent des blocs solides
    return (bloc == '#' || bloc == '|' || bloc == '_' || bloc == '0' || bloc == '+');
}


void appliquer_gravite(int en_saut, int camera_x, int* position_y, char map[H_MAP][L_MAP]) {
    int map_x = camera_x + PERSO_X;

    // Si le joueur n'est pas en saut et qu'il n'y a pas de bloc solide en dessous
    if (!en_saut && !est_solide(map_x, *position_y + 1, map)) {
        (*position_y)++; // Faire tomber le joueur
    }
}


void dessiner(int camera_x, int position_y, char map[H_MAP][L_MAP], int en_saut, int phase_saut, int nbPiece, int score) {
    printf("\033[H"); // Curseur en haut

    scoreboard(nbPiece, score);
    printf("\033[6;1H");
    char ligne[L_SCREEN + 1];
    ligne[L_SCREEN] = '\0';

    for (int i = 0; i < H_SCREEN; i++) {
        for (int j = 0; j < L_SCREEN; j++) {
            // Position dans la carte
            int map_x = j + camera_x;

            // Dessiner le personnage à sa position
            if (j == PERSO_X && (i == position_y || i == position_y - 1 || i == position_y - 2)) {
                if (i == position_y) ligne[j] = '^';
                else if (i == position_y - 1) ligne[j] = 'T';
                else if (i == position_y - 2) ligne[j] = 'O';
            }
            else if (map_x >= 0 && map_x < L_MAP) {
                ligne[j] = map[i][map_x];
            }
            else {
                ligne[j] = ' ';
            }
        }
        printf("%s\n", ligne);
    }
}