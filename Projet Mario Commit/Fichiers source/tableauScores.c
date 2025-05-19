#include "tableauScores.h"
#include "tbScore.h"
#include <stdbool.h>

Score tableau_scores[MAX_SCORES];
int nombre_scores = 0;

void reinitialiser_scores() {
    for (int i = 0; i < MAX_SCORES; i++) {
        strcpy_s(tableau_scores[i].nom, MAX_NOM_JOUEUR, "");
        strcpy_s(tableau_scores[i].perso, MAX_NOM_PERSO, "");
        tableau_scores[i].pieces = 0;
        tableau_scores[i].score = 0;
    }
    nombre_scores = 0;

    FILE* fichier;
    errno_t err = fopen_s(&fichier, FICHIER_SCORES, "wb");
    if (err == 0 && fichier != NULL) {
        fwrite(&nombre_scores, sizeof(int), 1, fichier);
        fclose(fichier);
    }

    printf("Tableau des scores reinitialise!\n");
}

void initialiser_tableau_scores() {
    for (int i = 0; i < MAX_SCORES; i++) {
        strcpy_s(tableau_scores[i].nom, MAX_NOM_JOUEUR, "");
        tableau_scores[i].pieces = 0;
        tableau_scores[i].score = 0;
    }
    nombre_scores = 0;

    charger_scores();
}

void charger_scores() {
    FILE* fichier;
    errno_t err = fopen_s(&fichier, FICHIER_SCORES, "rb");

    if (err == 0 && fichier != NULL) {
        fseek(fichier, 0, SEEK_END);
        long taille_fichier = ftell(fichier);
        fseek(fichier, 0, SEEK_SET);

        if (taille_fichier < sizeof(int)) {
            fclose(fichier);
            printf("Fichier de scores corrompu. Reinitialisation...\n");
            reinitialiser_scores();
            return;
        }

        if (fread(&nombre_scores, sizeof(int), 1, fichier) != 1) {
            fclose(fichier);
            printf("Erreur de lecture. Reinitialisation...\n");
            reinitialiser_scores();
            return;
        }

        if (nombre_scores < 0 || nombre_scores > MAX_SCORES) {
            fclose(fichier);
            printf("Nombre de scores invalide (%d). Reinitialisation...\n", nombre_scores);
            reinitialiser_scores();
            return;
        }

        long taille_attendue = sizeof(int) + (nombre_scores * sizeof(Score));
        if (taille_fichier != taille_attendue) {
            fclose(fichier);
            printf("Taille de fichier incoherente. Reinitialisation...\n");
            reinitialiser_scores();
            return;
        }

        if (fread(tableau_scores, sizeof(Score), nombre_scores, fichier) != nombre_scores) {
            fclose(fichier);
            printf("Erreur de lecture des scores. Reinitialisation...\n");
            reinitialiser_scores();
            return;
        }

        bool donnees_valides = true;
        for (int i = 0; i < nombre_scores; i++) {
            if (tableau_scores[i].score < 0 || tableau_scores[i].score > 1000000 ||
                tableau_scores[i].pieces < 0 || tableau_scores[i].pieces > 10000) {
                donnees_valides = false;
                break;
            }
        }

        if (!donnees_valides) {
            printf("Donnees de scores invalides detectees. Reinitialisation...\n");
            reinitialiser_scores();
        }

        fclose(fichier);
    }
    else {
    }
}

void sauvegarder_scores() {
    FILE* fichier;
    errno_t err = fopen_s(&fichier, FICHIER_SCORES, "wb");

    if (err == 0 && fichier != NULL) {
        fwrite(&nombre_scores, sizeof(int), 1, fichier);

        fwrite(tableau_scores, sizeof(Score), nombre_scores, fichier);

        fclose(fichier);
    }
}

void sauvegarder_score_actuel(int pieces, int score) {
    int pieces_actuelles = pieces;
    int score_actuel = score;
}

void ajouter_score(const char* nom, int pieces, int score) {
    if (nombre_scores < MAX_SCORES || score > tableau_scores[nombre_scores - 1].score) {
        Score nouveau_score;
        strcpy_s(nouveau_score.nom, MAX_NOM_JOUEUR, nom);
        nouveau_score.pieces = pieces;
        nouveau_score.score = score;

        if (nombre_scores < MAX_SCORES) {
            tableau_scores[nombre_scores] = nouveau_score;
            nombre_scores++;
        }
        else {
            tableau_scores[MAX_SCORES - 1] = nouveau_score;
        }

        trier_scores();

        sauvegarder_scores();
    }
}

void trier_scores() {
    for (int i = 0; i < nombre_scores - 1; i++) {
        for (int j = 0; j < nombre_scores - i - 1; j++) {
            if (tableau_scores[j].score < tableau_scores[j + 1].score) {
                Score temp = tableau_scores[j];
                tableau_scores[j] = tableau_scores[j + 1];
                tableau_scores[j + 1] = temp;
            }
            else if (tableau_scores[j].score == tableau_scores[j + 1].score &&
                tableau_scores[j].pieces < tableau_scores[j + 1].pieces) {
                Score temp = tableau_scores[j];
                tableau_scores[j] = tableau_scores[j + 1];
                tableau_scores[j + 1] = temp;
            }
        }
    }
}

void afficher_scores() {
    effacer_ecran();

    int largeur_console, hauteur_console;
    obtenir_taille_console(&largeur_console, &hauteur_console);

    int centre_titre = (largeur_console - strlen("=== TABLEAU DES SCORES ===")) / 2;
    for (int i = 0; i < centre_titre; i++) printf(" ");
    printf("=== TABLEAU DES SCORES ===\n\n");

    printf("%-5s %-25s %-10s %-10s\n", "Rang", "Nom", "Pieces", "Score");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < nombre_scores; i++) {
        printf("%-5d %-25s %-10d %-10d\n",
            i + 1,
            tableau_scores[i].nom,
            tableau_scores[i].pieces,
            tableau_scores[i].score);
    }

    printf("\n\nAppuyez sur une touche pour revenir au menu...");
    _getch();
}

void saisir_nouveau_score(int pieces, int score) {
    effacer_ecran();

    if (nombre_scores == MAX_SCORES && score <= tableau_scores[MAX_SCORES - 1].score) {
        printf("Votre score n'est pas assez eleve pour figurer au tableau des meilleurs scores.\n");
        printf("Pieces: %d, Score: %d\n\n", pieces, score);
        printf("Appuyez sur une touche pour continuer...");
        _getch();
        return;
    }

    char nom[MAX_NOM_JOUEUR];
    printf("=== NOUVEAU SCORE ===\n\n");
    printf("Bravo! Votre score peut figurer au tableau des meilleurs scores!\n");
    printf("Pieces: %d, Score: %d\n\n", pieces, score);
    printf("Entrez votre nom: ");
    scanf_s("%49s", nom, (unsigned)_countof(nom));

    ajouter_score(nom, pieces, score);

    printf("\nScore enregistre!\n");
    printf("Appuyez sur une touche pour voir le tableau des scores...");
    _getch();

    afficher_scores();
}

void menu_tableau_scores() {
    afficher_scores();
}