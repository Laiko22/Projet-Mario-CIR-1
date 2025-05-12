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

// Fonction pour tester si un bloc est solide
int est_solide(int x, int y, char map[H_MAP][L_MAP]) {
    // Vérifier si la position est en dehors de la carte
    if (x < 0 || x >= L_MAP || y < 0 || y >= H_MAP) {
        return 1; // Considérer les bords comme solides
    }

    // Vérifier le type de bloc à cette position
    char bloc = map[y][x];

    // Définir quels caractères représentent des blocs solides
    // Par exemple, '#' pour un bloc solide, ' ' pour de l'air (ajustez selon votre carte)
    return (bloc == '#' || bloc == '=' || bloc == '[' || bloc == ']' || bloc == '-');
}

// Fonction pour appliquer la gravité - modifiée pour utiliser un pointeur
void appliquer_gravite(int en_saut, int camera_x, int* position_y, char map[H_MAP][L_MAP]) {
    int map_x = camera_x + PERSO_X;

    // Si le joueur n'est pas en saut et qu'il n'y a pas de bloc solide en dessous
    if (!en_saut && !est_solide(map_x, *position_y + 1, map)) {
        (*position_y)++; // Faire tomber le joueur
    }
}

// Fonction pour dessiner l'écran de jeu
void dessiner(int camera_x, int position_y, char map[H_MAP][L_MAP], int en_saut, int phase_saut) {
    printf("\033[H"); // Curseur en haut

    char ligne[L_SCREEN + 1];
    ligne[L_SCREEN] = '\0';

    for (int i = 0; i < H_SCREEN; i++) {
        for (int j = 0; j < L_SCREEN; j++) {
            // Position dans la carte
            int map_x = j + camera_x;

            // Dessiner le personnage à sa position
            if (i == position_y && j == PERSO_X) {
                if (en_saut) {
                    ligne[j] = '^'; // Personnage en saut
                }
                else {
                    ligne[j] = 'M'; // Personnage normal
                }
            }
            else if (map_x < L_MAP && i < H_MAP) {
                ligne[j] = map[i][map_x];
            }
            else {
                ligne[j] = ' '; // Espace vide si en dehors des limites
            }
        }
        printf("%s\n", ligne);
    }
}