#include "menu.h"

char obtenir_touche() {
    if (_kbhit()) {
        return _getch();
    }
    else {
        return 0;
    }
}

void pause_console() { // mettre en pause
    printf("\nAppuyez sur une touche pour continuer...\n");
    obtenir_touche();
}

void effacer_ecran() {// effacer l'ecran 
    system("cls");
}

void afficher_menu_principal(int selection) {
    static int affichage_initial = 1;
    static int derniere_selection = -1;

    if (affichage_initial || derniere_selection != selection) {
        effacer_ecran();
        char* menu[] = { "Nouvelle partie", "Sauvegardes", "Tableau des scores", "Quitter" };
        int nb_options = 4;

        for (int i = 0; i < nb_options; i++) {
            if (i == selection) {
                printf("%*s+------------------------------------------+\n", CENTRE_X, "");
                printf("%*s|              [ %s ]                      |\n", CENTRE_X, "", menu[i]);
                printf("%*s+------------------------------------------+\n", CENTRE_X, "");
            }
            else {
                printf("%*s+------------------------------------------+\n", CENTRE_X, "");
                printf("%*s|                %s                        |\n", CENTRE_X, "", menu[i]);
                printf("%*s+------------------------------------------+\n", CENTRE_X, "");
            }
        }

        affichage_initial = 0;
        derniere_selection = selection;
    }
}

int gestion_touches_fleche(int* selection, int max) {
    char cle = obtenir_touche();
    if (cle == -32 || cle == 0) {
        cle = obtenir_touche();
        if (cle == 72 && *selection > 0) { //fleche haut
            (*selection)--;
        }
        else if (cle == 80 && *selection < max) { // fleche bas
            (*selection)++;
        }
    }
    else if (cle == 27) { // echap
        return -1;
    }
    return 0;
}

void choix_pseudo(char* pseudo) {
    effacer_ecran();
    printf("=== Choix du pseudo ===\n\n");
    printf("Entrez votre pseudo : ");
    scanf_s("%s", pseudo, 50);
    pause_console();
}

void choix_personnage(char* perso) {
    char* persos[NB_PERSONNAGES] = { "Mario", "Luigi", "Toad", "Yoshi" };
    int selection = 0;
    char cle;

    while (1) {
        effacer_ecran();
        printf("=== Choix du personnage ===\n\n");

        for (int i = 0; i < NB_PERSONNAGES; i++) {
            if (i == selection) {
                printf("%*s+------------------------------------------+\n", CENTRE_X, "");
                printf("%*s|              [ %s ]                      |\n", CENTRE_X, "", persos[i]);
                printf("%*s+------------------------------------------+\n", CENTRE_X, "");
            }
            else {
                printf("%*s+------------------------------------------+\n", CENTRE_X, "");
                printf("%*s|                %s                        |\n", CENTRE_X, "", persos[i]);
                printf("%*s+------------------------------------------+\n", CENTRE_X, "");
            }
        }

        cle = _getch();

        if (cle == -32 || cle == 0) {
            cle = _getch();
            if (cle == 72 && selection > 0) {
                selection--;
            }
            else if (cle == 80 && selection < NB_PERSONNAGES - 1) {
                selection++;
            }
        }
        else if (cle == 27) { // Échap
            return;
        }
        else if (cle == 13) { // Entrée
            strcpy_s(perso, 50, persos[selection]);
            break;
        }
    }

    pause_console();
}

void nouvelle_partie() {
    char pseudo[50];
    char perso[50];
    choix_pseudo(pseudo);
    choix_personnage(perso);
    effacer_ecran();
    printf("Nouvelle partie lancee avec %s (%s) !\n", pseudo, perso);
    pause_console();
}

void executer_menu_principal() {
    int selection = 0;
    char cle;
    afficher_menu_principal(selection);

    while (1) {
        cle = _getch();
        if (cle == -32 || cle == 0) {
            cle = _getch();
            int ancienne_selection = selection;
            if (cle == 72 && selection > 0) selection--;
            else if (cle == 80 && selection < 3) selection++;
            if (ancienne_selection != selection) {
                afficher_menu_principal(selection);
            }
        }
        else if (cle == 13) {
            switch (selection) {
            case 0:
                nouvelle_partie();
                afficher_menu_principal(selection);
                break;
            case 1:
                // Gestion des sauvegardes
                break;
            case 2:
                // Tableau des scores
                break;
            case 3:
                effacer_ecran();
                printf("Au revoir !\n");
                return;
            }
        }
        Sleep(10);
    }
}