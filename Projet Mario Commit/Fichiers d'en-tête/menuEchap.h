#ifndef MENUECHAP_H
#define MENUECHAP_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include "mario.h" 

#define LARGEUR_MENU 40


char obtenir_toucheE();
void pause_consoleE();
void obtenir_taille_consoleE(int* largeur, int* hauteur);
void effacer_ecranE();
int gestion_touches_flecheE(int* selection, int max);


void afficher_menuEchap(int selection);

int menuEchap(char map[H_MAP][L_MAP]);

#endif