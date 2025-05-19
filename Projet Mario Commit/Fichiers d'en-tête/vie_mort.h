#ifndef VIE_MORT_H
#define VIE_MORT_H

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "menu.h"
#include "mario.h"
#include "jeu.h"

#define NB_VIES_INITIAL 3

void init_vie_state(JeuState* etat);


int est_mort(JeuState* etat, char map[H_MAP][L_MAP], Goomba* goombas, int nb_goombas);

int menu_mort(JeuState* etat);

void respawn_joueur(JeuState* etat);

void afficher_vies(JeuState* etat);

#endif