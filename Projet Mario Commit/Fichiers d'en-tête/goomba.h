#ifndef GOOMBA_H
#define GOOMBA_H

#include <windows.h>
#include <stdio.h>

#define H_SCREEN 19
#define L_SCREEN 156
#define H_MAP 19
#define L_MAP 469
#define MAX_GOOMBAS 10
#define VITESSE_GOOMBA 2


typedef struct {
	int x;
	int y;
	int direction;
	int borne_gauche;
	int borne_droite;
	int compteur;
} Goomba;

void initialiser_goomba(Goomba* g, int x, int y);
void deplacer_goomba(Goomba* g, char map[H_MAP][L_MAP]);
void dessiner_goomba(Goomba g, int camera_x, HANDLE hConsole, int i, int j);

#endif