#ifndef JEU_H
#define JEU_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "mario.h"
#include "goomba.h"

#define VITESSE_DEPLACEMENT 1

typedef struct {
	int position_y;       // Position verticale du personnage
	int position_x;       // Position horizontale du personnage (relatif à l'écran)
	int camera_x;         // Position horizontale de la caméra
	int en_saut;          // Indique si le personnage est en train de sauter
	int phase_saut;       // Phase du saut (0-11 pour un saut complet)
	int quitter;          // Indique si le joueur veut quitter le jeu
	int nbPiece;          // Nombre de pièces collectées
} JeuState;

void init_jeu(JeuState* etat, char map[H_MAP][L_MAP]);
void gestion_clavier(JeuState* etat, char map[H_MAP][L_MAP], int nbPiece, int score, Goomba g);
void mise_a_jour_jeu(JeuState* etat, char map[H_MAP][L_MAP]);
void verifier_collision_pieces(JeuState* etat, char map[H_MAP][L_MAP]);
void boucle_jeu(char map[H_MAP][L_MAP], int nbPiece, int score, Goomba g);

#endif