#ifndef TABLEAUSCORES_H
#define TABLEAUSCORES_H

#include <stdio.h>
#include <string.h>

#define MAX_SCORES 10
#define MAX_NOM_JOUEUR 50
#define MAX_NOM_PERSO 20
#define FICHIER_SCORES "scores.dat"

typedef struct {
    char nom[MAX_NOM_JOUEUR];
    char perso[MAX_NOM_PERSO];
    int pieces;
    int score;
} Score;

void initialiser_tableau_scores();
void charger_scores();
void sauvegarder_scores();
void ajouter_score(const char* nom, const char* perso, int pieces, int score);
void trier_scores();
void afficher_scores();
void menu_tableau_scores();

void definir_joueur_actuel(const char* pseudo, const char* perso);
char* obtenir_pseudo_actuel();
char* obtenir_perso_actuel();

void sauvegarder_score_actuel(int pieces, int score);
int obtenir_pieces_actuelles();
int obtenir_score_actuel();

void saisir_nouveau_score(int pieces, int score);

#endif