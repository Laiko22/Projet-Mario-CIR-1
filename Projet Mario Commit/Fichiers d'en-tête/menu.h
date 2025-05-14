#ifndef MENU_H
#define MENU_H
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>


#define NB_PERSONNAGES 4
#define LARGEUR_MENU 44

typedef struct {
    char nom[50];
    int score;
} Joueur;

char obtenir_touche();
void pause_console();
void effacer_ecran();
void afficher_menu_principal(int selection);
int gestion_touches_fleche(int* selection, int max);
void obtenir_taille_console(int* largeur, int* hauteur);
void choix_pseudo(char* pseudo);
void choix_personnage(char* perso);
void nouvelle_partie();
void executer_menu_principal();

#endif