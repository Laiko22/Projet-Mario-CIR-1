#ifndef MARIO_H
#define MARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "goomba.h"

#define H_SCREEN 19
#define L_SCREEN 156
#define H_MAP 19
#define L_MAP 469
#define HAUTEUR_SAUT 10
#define PERSO_X 78
#define VITESSE_DEPLACEMENT 1

void affichage(char map[H_MAP][L_MAP]);
void commande(int touche, char map[H_MAP][L_MAP]);
void dessiner(int camera_x, int position_y, char map[H_MAP][L_MAP], int en_saut, int phase_saut, int nbPiece, int score, Goomba g);
void dessiner_avec_position_x(int camera_x, int position_y, int position_x, char map[H_MAP][L_MAP], int en_saut, int phase_saut, int nbPiece, int score, Goomba g);
int est_solide(int x, int y, char map[H_MAP][L_MAP]);
void appliquer_gravite(int en_saut, int camera_x, int* position_y, char map[H_MAP][L_MAP]);

#endif