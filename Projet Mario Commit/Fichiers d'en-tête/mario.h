/* mario.h */

#ifndef MARIO_H
#define MARIO_H
#define H_SCREEN 19
#define L_SCREEN 156
#define H_MAP 19
#define L_MAP 469
#define HAUTEUR_SAUT 10
#define PERSO_X 78
#define VITESSE_DEPLACEMENT 1

#include <stdio.h>
#include <conio.h>
#include <windows.h>

void affichage(char map[H_MAP][L_MAP]);
void commande(int touche, char map[H_MAP][L_MAP]);
void dessiner(int camera_x, int position_y, char map[H_MAP][L_MAP], int en_saut, int phase_saut, int nbPiece, int score);
int est_solide(int x, int y, char map[H_MAP][L_MAP]);
void appliquer_gravite(int en_saut, int camera_x, int* position_y, char map[H_MAP][L_MAP]);
void scoreboard(int nbPiece, int score);
#endif