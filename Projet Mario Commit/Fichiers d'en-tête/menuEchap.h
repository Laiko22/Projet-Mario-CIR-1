#ifndef MENUECHAP_H
#define MENUECHAP_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include "mario.h" // Pour accéder aux dimensions du jeu

#define LARGEUR_MENU 40

// Fonctions utilitaires pour le menu d'échap
char obtenir_toucheE();
void pause_consoleE();
void obtenir_taille_consoleE(int* largeur, int* hauteur);
void effacer_ecranE();
int gestion_touches_flecheE(int* selection, int max);

// Fonction pour afficher le menu d'échap
void afficher_menuEchap(int selection);

// Fonction principale pour gérer le menu d'échap 
// Retourne 0 pour reprendre le jeu, 1 pour quitter le jeu
int menuEchap(char map[H_MAP][L_MAP]);

#endif
