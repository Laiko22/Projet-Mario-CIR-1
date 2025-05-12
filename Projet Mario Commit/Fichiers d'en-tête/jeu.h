#ifndef JEU_H
#define JEU_H

#include "mario.h"
#include "menu.h"

typedef struct {
    int position_y;
    int camera_x;
    int en_saut;
    int phase_saut;
    int quitter;
} JeuState;

void init_jeu(JeuState* etat, char map[H_MAP][L_MAP]);
void gestion_clavier(JeuState* etat, char map[H_MAP][L_MAP]);
void mise_a_jour_jeu(JeuState* etat, char map[H_MAP][L_MAP]);
void boucle_jeu(char map[H_MAP][L_MAP]);

#endif
