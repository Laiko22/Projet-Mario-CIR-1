#ifndef VIE_MORT_H
#define VIE_MORT_H

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "menu.h"
#include "mario.h"
#include "jeu.h"

#define NB_VIES_INITIAL 3

// Initialisation des vies
void init_vie_state(JeuState* etat);

// Vérifie si le joueur est mort (chute dans le vide ou ennemi)
int est_mort(JeuState* etat, char map[H_MAP][L_MAP], Goomba* goombas, int nb_goombas);

// Affiche le menu de mort lorsqu'une vie est perdue
int menu_mort(JeuState* etat);

// Fonction pour réinitialiser la position du joueur
void respawn_joueur(JeuState* etat);

// Fonction pour afficher les vies restantes pendant le jeu
void afficher_vies(JeuState* etat);

#endif