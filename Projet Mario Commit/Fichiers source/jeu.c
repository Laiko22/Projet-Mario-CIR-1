#include "jeu.h"

void init_jeu(JeuState* etat, char map[H_MAP][L_MAP]) {
    etat->position_y = 14;
    etat->camera_x = 0;
    etat->en_saut = 0;
    etat->phase_saut = 0;
    etat->quitter = 0;
    etat->nbPiece = 0;
}

void gestion_clavier(JeuState* etat, char map[H_MAP][L_MAP], int nbPiece, int score) {
    if (_kbhit()) {
        char touche = _getch();
        if (touche == 0 || touche == -32) {
            char fleche = _getch();
            // Fl�che gauche
            if (fleche == 75 && etat->camera_x > 0) {
                // V�rifier si le d�placement est possible (pas de mur)
                int nouvelle_pos_x = etat->camera_x - VITESSE_DEPLACEMENT;
                int map_x = nouvelle_pos_x + PERSO_X;
                // V�rifier si le personnage ne va pas traverser un mur
                if (!est_solide(map_x, etat->position_y, map) &&
                    !est_solide(map_x, etat->position_y - 1, map) &&
                    !est_solide(map_x, etat->position_y - 2, map)) {
                    etat->camera_x = nouvelle_pos_x;
                    verifier_collision_pieces(etat, map);
                }
            }
            // Fl�che droite
            if (fleche == 77 && etat->camera_x < L_MAP - L_SCREEN) {
                // V�rifier si le d�placement est possible (pas de mur)
                int nouvelle_pos_x = etat->camera_x + VITESSE_DEPLACEMENT;
                int map_x = nouvelle_pos_x + PERSO_X;
                // V�rifier si le personnage ne va pas traverser un mur
                if (!est_solide(map_x, etat->position_y, map) &&
                    !est_solide(map_x, etat->position_y - 1, map) &&
                    !est_solide(map_x, etat->position_y - 2, map)) {
                    etat->camera_x = nouvelle_pos_x;
                    verifier_collision_pieces(etat, map);
                }
            }
            // Fl�che haut (pour sauter)
            if (fleche == 72 && !etat->en_saut) {
                int map_x = etat->camera_x + PERSO_X;
                if (est_solide(map_x, etat->position_y + 1, map)) {
                    etat->en_saut = 1;
                    etat->phase_saut = 0;
                }
            }
        }
        // Barre d'espace (pour sauter �galement)
        else if (touche == 32 && !etat->en_saut) {
            int map_x = etat->camera_x + PERSO_X;
            if (est_solide(map_x, etat->position_y + 1, map)) {
                etat->en_saut = 1;
                etat->phase_saut = 0;
            }
        }
        // Touche �chap (pour acc�der au menu pause)
        else if (touche == 27) {
            int resultat_menu = menuEchap(map);
            if (resultat_menu == 1) {
                etat->quitter = 1; // Quitter le jeu et retourner au menu principal
                return; // Sortir imm�diatement de la fonction
            }
            // Redessiner l'�cran du jeu apr�s retour du menu
            effacer_ecranE();
            dessiner(etat->camera_x, etat->position_y, map, etat->en_saut, etat->phase_saut, nbPiece, score);
        }
    }
}

void mise_a_jour_jeu(JeuState* etat, char map[H_MAP][L_MAP]) {
    if (etat->en_saut) {
        int map_x = etat->camera_x + PERSO_X;
        if (etat->phase_saut < HAUTEUR_SAUT) {
            // V�rifier si le personnage peut monter (pas de plafond)
            if (!est_solide(map_x, etat->position_y - 3, map)) {
                etat->position_y--;
                verifier_collision_pieces(etat, map);
            }
            else {
                // Collision avec un plafond, arr�ter la mont�e
                etat->phase_saut = HAUTEUR_SAUT;
            }
            etat->phase_saut++;
        }
        else if (etat->phase_saut < 2 * HAUTEUR_SAUT) {
            // V�rifier si le personnage peut descendre (pas de sol)
            if (!est_solide(map_x, etat->position_y + 1, map)) {
                etat->position_y++;
                verifier_collision_pieces(etat, map);
            }
            else {
                // Collision avec le sol, arr�ter le saut
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
        verifier_collision_pieces(etat, map);
    }
}

void verifier_collision_pieces(JeuState* etat, char map[H_MAP][L_MAP]) {
    int map_x = etat->camera_x + PERSO_X;

    // V�rifier pour les trois parties du corps du personnage (t�te, torse, pieds)
    for (int partie = 0; partie < 3; partie++) {
        int pos_y = etat->position_y - partie;

        // Si la position est valide
        if (pos_y >= 0 && pos_y < H_MAP && map_x >= 0 && map_x < L_MAP) {
            // Si on trouve une pi�ce � cette position
            if (map[pos_y][map_x] == 'o') {
                // Collecte de la pi�ce
                map[pos_y][map_x] = '.';  // Remplacer la pi�ce par un espace vide
                etat->nbPiece++;  // Incr�menter le compteur de pi�ces
            }
        }
    }
}

void boucle_jeu(char map[H_MAP][L_MAP], int nbPiece, int score) {
    JeuState etat;
    init_jeu(&etat, map);
    printf("\033[2J\033[H");
    affichage(map);
    while (!etat.quitter) {
        gestion_clavier(&etat, map, nbPiece, score);
        mise_a_jour_jeu(&etat, map);
        dessiner(etat.camera_x, etat.position_y, map, etat.en_saut, etat.phase_saut, nbPiece, score);
        Sleep(70);
    }
    // Ne pas retourner de valeur car la fonction est de type void
}