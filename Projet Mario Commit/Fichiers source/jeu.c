#include "jeu.h"

void init_jeu(JeuState* etat, char map[H_MAP][L_MAP]) {
    etat->position_y = 14;
    etat->camera_x = 0;
    etat->en_saut = 0;
    etat->phase_saut = 0;
    etat->quitter = 0;
}

void gestion_clavier(JeuState* etat, char map[H_MAP][L_MAP]) {
    if (_kbhit()) {
        char touche = _getch();
        if (touche == 0 || touche == -32) {
            char fleche = _getch();
            // Flèche gauche
            if (fleche == 75 && etat->camera_x > 0) {
                etat->camera_x -= VITESSE_DEPLACEMENT;
            }
            // Flèche droite
            if (fleche == 77 && etat->camera_x < L_MAP - L_SCREEN) {
                etat->camera_x += VITESSE_DEPLACEMENT;
            }
            // Flèche haut (pour sauter)
            if (fleche == 72 && !etat->en_saut) {
                int map_x = etat->camera_x + PERSO_X;
                if (est_solide(map_x, etat->position_y + 1, map)) {
                    etat->en_saut = 1;
                    etat->phase_saut = 0;
                }
            }
        }
        // Barre d'espace (pour sauter également)
        else if (touche == 32 && !etat->en_saut) {
            int map_x = etat->camera_x + PERSO_X;
            if (est_solide(map_x, etat->position_y + 1, map)) {
                etat->en_saut = 1;
                etat->phase_saut = 0;
            }
        }
        else if (touche == 'q' || touche == 'Q') {
            etat->quitter = 1;
        }
    }
}

void mise_a_jour_jeu(JeuState* etat, char map[H_MAP][L_MAP]) {
    if (etat->en_saut) {
        int map_x = etat->camera_x + PERSO_X;
        if (etat->phase_saut < HAUTEUR_SAUT) {
            if (!est_solide(map_x, etat->position_y - 1, map)) {
                etat->position_y--;
            }
            else {
                etat->phase_saut = HAUTEUR_SAUT;
            }
            etat->phase_saut++;
        }
        else if (etat->phase_saut < 2 * HAUTEUR_SAUT) {
            if (!est_solide(map_x, etat->position_y + 1, map)) {
                etat->position_y++;
            }
            else {
                etat->en_saut = 0;
            }
            etat->phase_saut++;
        }
        else {
            etat->en_saut = 0;
        }
    }
    else {
        appliquer_gravite(etat->en_saut, etat->camera_x, &etat->position_y, map);
    }
}

void boucle_jeu(char map[H_MAP][L_MAP]) {
    JeuState etat;
    init_jeu(&etat, map);

    printf("\033[2J\033[H");
    affichage(map);

    while (!etat.quitter) {
        gestion_clavier(&etat, map);
        mise_a_jour_jeu(&etat, map);
        dessiner(etat.camera_x, etat.position_y, map, etat.en_saut, etat.phase_saut);
        Sleep(70);
    }
}