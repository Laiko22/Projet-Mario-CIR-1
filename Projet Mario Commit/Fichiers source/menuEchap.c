﻿#include "menuEchap.h"

char obtenir_toucheE() {
    if (_kbhit()) {
        return _getch();
    }
    else {
        return 0;
    }
}

void pause_consoleE() {
    printf("\nAppuyez sur une touche pour continuer...\n");
    obtenir_toucheE();
}

void obtenir_taille_consoleE(int* largeur, int* hauteur) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    *largeur = info.srWindow.Right - info.srWindow.Left + 1;
    *hauteur = info.srWindow.Bottom - info.srWindow.Top + 1;
}

void effacer_ecranE() {
    system("cls");
}

int gestion_touches_flecheE(int* selection, int max) {
    char cle = obtenir_toucheE();
    if (cle == -32 || cle == 0) {
        cle = obtenir_toucheE();
        if (cle == 72 && *selection > 0) {
            (*selection)--;
        }
        else if (cle == 80 && *selection < max) {
            (*selection)++;
        }
    }
    else if (cle == 27) {
        return -1;
    }
    return 0;
}

void afficher_menuEchap(int selection) {
    effacer_ecranE();

    char* menu[] = { "Reprendre", "Sauvegarder", "Quitter" };
    int nb_options = 3;
    int largeur_console, hauteur_console;

    obtenir_taille_consoleE(&largeur_console, &hauteur_console);

    int centre_x = (largeur_console - LARGEUR_MENU) / 2;
    int lignes_par_option = 4;
    int total_lignes_menu = nb_options * lignes_par_option;
    int centre_y = (hauteur_console - total_lignes_menu) / 2;

    for (int i = 0; i < centre_y; i++) printf("\n");

    for (int i = 0; i < nb_options; i++) {
        printf("%*s+------------------------------------+\n", centre_x, "");
        printf("%*s|", centre_x, "");

        if (i == selection) {
            int len = strlen(menu[i]);
            int espaces = (LARGEUR_MENU - 4 - len - 4) / 2;
            for (int j = 0; j < espaces; j++) printf(" ");
            printf("[ %s ]", menu[i]);
            for (int j = 0; j < LARGEUR_MENU - 4 - len - 4 - espaces; j++) printf(" ");
        }
        else {
            int len = strlen(menu[i]);
            int espaces = (LARGEUR_MENU - 4 - len) / 2;
            for (int j = 0; j < espaces; j++) printf(" ");
            printf("%s", menu[i]);
            for (int j = 0; j < LARGEUR_MENU - 4 - len - espaces; j++) printf(" ");
        }

        printf("|\n");
        printf("%*s+------------------------------------+\n", centre_x, "");
    }
}

int menuEchap(char map[H_MAP][L_MAP]) {
    int selection = 0;
    char cle;

    afficher_menuEchap(selection);

    while (1) {
        if (_kbhit()) {
            cle = _getch();
            if (cle == -32 || cle == 0) {
                cle = _getch();
                int ancienne_selection = selection;
                if (cle == 72 && selection > 0) selection--;
                else if (cle == 80 && selection < 2) selection++;

                if (ancienne_selection != selection) {
                    afficher_menuEchap(selection);
                }
            }
            else if (cle == 13) {
                switch (selection) {
                case 0:
                    effacer_ecranE();
                    return 0;
                case 1:
                    printf("Sauvegarde en cours...\n");
                    Sleep(1000);
                    afficher_menuEchap(selection);
                    break;
                case 2:
                    effacer_ecranE();

                    return 1;
                }
            }
            else if (cle == 27) {
                effacer_ecranE();
                return 0;
            }
        }
        Sleep(10);
    }
}