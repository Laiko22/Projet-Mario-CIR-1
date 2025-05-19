#include "tableauScores.h"
#include "tbScore.h"
#include <stdbool.h>
// Tableau global pour stocker les scores
Score tableau_scores[MAX_SCORES];
int nombre_scores = 0;
void reinitialiser_scores() {
    // Reinitialiser toutes les entrees
    for (int i = 0; i < MAX_SCORES; i++) {
        strcpy_s(tableau_scores[i].nom, MAX_NOM_JOUEUR, "");
        strcpy_s(tableau_scores[i].perso, MAX_NOM_PERSO, "");
        tableau_scores[i].pieces = 0;
        tableau_scores[i].score = 0;
    }
    nombre_scores = 0;

    // Supprimer le fichier existant et en creer un nouveau vide
    FILE* fichier;
    errno_t err = fopen_s(&fichier, FICHIER_SCORES, "wb");
    if (err == 0 && fichier != NULL) {
        // ecrire un tableau vide
        fwrite(&nombre_scores, sizeof(int), 1, fichier);
        fclose(fichier);
    }

    printf("Tableau des scores reinitialise!\n");
}
// Initialise le tableau des scores
void initialiser_tableau_scores() {
    for (int i = 0; i < MAX_SCORES; i++) {
        strcpy_s(tableau_scores[i].nom, MAX_NOM_JOUEUR, "");
        tableau_scores[i].pieces = 0;
        tableau_scores[i].score = 0;
    }
    nombre_scores = 0;

    // Essayer de charger les scores à partir du fichier
    charger_scores();
}

// Charge les scores à partir du fichier
void charger_scores() {
    FILE* fichier;
    errno_t err = fopen_s(&fichier, FICHIER_SCORES, "rb");

    if (err == 0 && fichier != NULL) {
        // Obtenir la taille du fichier
        fseek(fichier, 0, SEEK_END);
        long taille_fichier = ftell(fichier);
        fseek(fichier, 0, SEEK_SET);

        // Verifier que le fichier a une taille coherente
        if (taille_fichier < sizeof(int)) {
            fclose(fichier);
            printf("Fichier de scores corrompu. Reinitialisation...\n");
            reinitialiser_scores();
            return;
        }

        // Lire le nombre de scores
        if (fread(&nombre_scores, sizeof(int), 1, fichier) != 1) {
            fclose(fichier);
            printf("Erreur de lecture. Reinitialisation...\n");
            reinitialiser_scores();
            return;
        }

        // Verifier que le nombre est valide
        if (nombre_scores < 0 || nombre_scores > MAX_SCORES) {
            fclose(fichier);
            printf("Nombre de scores invalide (%d). Reinitialisation...\n", nombre_scores);
            reinitialiser_scores();
            return;
        }

        // Verifier que la taille du fichier correspond
        long taille_attendue = sizeof(int) + (nombre_scores * sizeof(Score));
        if (taille_fichier != taille_attendue) {
            fclose(fichier);
            printf("Taille de fichier incoherente. Reinitialisation...\n");
            reinitialiser_scores();
            return;
        }

        // Lire les scores
        if (fread(tableau_scores, sizeof(Score), nombre_scores, fichier) != nombre_scores) {
            fclose(fichier);
            printf("Erreur de lecture des scores. Reinitialisation...\n");
            reinitialiser_scores();
            return;
        }

        // Validation supplementaire des donnees lues
        bool donnees_valides = true;
        for (int i = 0; i < nombre_scores; i++) {
            // Verifier que les scores et pièces sont dans des plages raisonnables
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
        // Si le fichier n'existe pas, ce n'est pas une erreur
        // Le tableau reste initialise à vide
    }
}
// Sauvegarde les scores dans un fichier
void sauvegarder_scores() {
    FILE* fichier;
    errno_t err = fopen_s(&fichier, FICHIER_SCORES, "wb");

    if (err == 0 && fichier != NULL) {
        // ecrire le nombre de scores
        fwrite(&nombre_scores, sizeof(int), 1, fichier);

        // ecrire les scores
        fwrite(tableau_scores, sizeof(Score), nombre_scores, fichier);

        fclose(fichier);
    }
}

// Fonction pour sauvegarder le score actuel pendant la partie
void sauvegarder_score_actuel(int pieces, int score) {
    int pieces_actuelles = pieces;
    int score_actuel = score;
}

// Ajoute un nouveau score et trie la liste
void ajouter_score(const char* nom, int pieces, int score) {
    // Verifier si le score merite d'être ajoute (s'il est meilleur que le dernier ou si on a moins de MAX_SCORES)
    if (nombre_scores < MAX_SCORES || score > tableau_scores[nombre_scores - 1].score) {
        // Creer un nouveau score
        Score nouveau_score;
        strcpy_s(nouveau_score.nom, MAX_NOM_JOUEUR, nom);
        nouveau_score.pieces = pieces;
        nouveau_score.score = score;

        // Ajouter le score
        if (nombre_scores < MAX_SCORES) {
            tableau_scores[nombre_scores] = nouveau_score;
            nombre_scores++;
        }
        else {
            // Remplacer le dernier score
            tableau_scores[MAX_SCORES - 1] = nouveau_score;
        }

        // Trier les scores
        trier_scores();

        // Sauvegarder dans le fichier
        sauvegarder_scores();
    }
}

// Trie les scores par ordre decroissant
void trier_scores() {
    // Tri à bulles avec critère principal: score, critère secondaire: pièces
    for (int i = 0; i < nombre_scores - 1; i++) {
        for (int j = 0; j < nombre_scores - i - 1; j++) {
            // Comparaison du score principal
            if (tableau_scores[j].score < tableau_scores[j + 1].score) {
                // echanger les scores
                Score temp = tableau_scores[j];
                tableau_scores[j] = tableau_scores[j + 1];
                tableau_scores[j + 1] = temp;
            }
            // En cas d'egalite de score, comparer les pièces
            else if (tableau_scores[j].score == tableau_scores[j + 1].score &&
                tableau_scores[j].pieces < tableau_scores[j + 1].pieces) {
                // echanger les scores
                Score temp = tableau_scores[j];
                tableau_scores[j] = tableau_scores[j + 1];
                tableau_scores[j + 1] = temp;
            }
        }
    }
}
// Affiche les scores dans une presentation soignee
void afficher_scores() {
    effacer_ecran();

    int largeur_console, hauteur_console;
    obtenir_taille_console(&largeur_console, &hauteur_console);

    // Titre
    int centre_titre = (largeur_console - strlen("=== TABLEAU DES SCORES ===")) / 2;
    for (int i = 0; i < centre_titre; i++) printf(" ");
    printf("=== TABLEAU DES SCORES ===\n\n");

    // Entête du tableau
    printf("%-5s %-25s %-10s %-10s\n", "Rang", "Nom", "Pieces", "Score");
    printf("---------------------------------------------------\n");

    // Afficher les scores
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

// Saisie d'un nouveau score
void saisir_nouveau_score(int pieces, int score) {
    effacer_ecran();

    // Verifier si le score merite d'être ajoute
    if (nombre_scores == MAX_SCORES && score <= tableau_scores[MAX_SCORES - 1].score) {
        printf("Votre score n'est pas assez eleve pour figurer au tableau des meilleurs scores.\n");
        printf("Pieces: %d, Score: %d\n\n", pieces, score);
        printf("Appuyez sur une touche pour continuer...");
        _getch();
        return;
    }

    // Demander le nom
    char nom[MAX_NOM_JOUEUR];
    printf("=== NOUVEAU SCORE ===\n\n");
    printf("Bravo! Votre score peut figurer au tableau des meilleurs scores!\n");
    printf("Pieces: %d, Score: %d\n\n", pieces, score);
    printf("Entrez votre nom: ");
    scanf_s("%49s", nom, (unsigned)_countof(nom));

    // Ajouter le score
    ajouter_score(nom, pieces, score);

    printf("\nScore enregistre!\n");
    printf("Appuyez sur une touche pour voir le tableau des scores...");
    _getch();

    // Afficher le tableau des scores
    afficher_scores();
}

// Menu du tableau des scores
void menu_tableau_scores() {
    afficher_scores();
}

// Affiche le scoreboard en jeu
