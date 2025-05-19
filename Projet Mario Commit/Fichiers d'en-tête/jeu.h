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
	int position_y;       
	int position_x;       
	int camera_x;        
	int en_saut;          
	int phase_saut;       
	int quitter;          
	int nbPiece;
	int direction;
	int nb_vies;            
	int position_respawn_x; 
	int position_respawn_y;
} JeuState;

void init_jeu(JeuState* etat, char map[H_MAP][L_MAP]);
void gestion_clavier(JeuState* etat, char map[H_MAP][L_MAP], int nbPiece, int score, Goomba* g, int nb_goombas);
void mise_a_jour_jeu(JeuState* etat, char map[H_MAP][L_MAP], int nbPiece, int score, Goomba* g, int nb_goombas);
int verifier_fin_niveau(JeuState* etat, char map[H_MAP][L_MAP]);
void deplacement_gauche(JeuState* etat, char map[H_MAP][L_MAP]);
void deplacement_droite(JeuState* etat, char map[H_MAP][L_MAP]);
void afficher_felicitations(int nbPiece, int score);
void verifier_collision_pieces(JeuState* etat, char map[H_MAP][L_MAP]);
void boucle_jeu(char map[H_MAP][L_MAP], int nbPiece, int score, Goomba* g, int nb_goombas);
void verifier_mort(JeuState* etat, char map[H_MAP][L_MAP], int nbPiece, int score, Goomba* g, int nb_goombas);

#endif