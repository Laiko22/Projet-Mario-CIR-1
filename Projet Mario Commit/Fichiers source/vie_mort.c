#include "vie_mort.h"

void init_vie_state(JeuState* etat) {
    etat->nb_vies = NB_VIES_INITIAL;
    etat->position_respawn_x = 0;
    etat->position_respawn_y = 14;
}

int est_mort(JeuState* etat, char map[H_MAP][L_MAP], Goomba* goombas, int nb_goombas) {
    int map_x = etat->camera_x + etat->position_x;
    int pos_y = etat->position_y;

    if (pos_y >= H_MAP - 1) {
        return 1;
    }

    for (int partie = 0; partie < 3; partie++) {
        int check_y = pos_y - partie;

        for (int i = 0; i < nb_goombas; i++) {
            if (goombas[i].x == map_x && goombas[i].y == check_y) {
                return 2;
            }
        }
    }

    return 0;
}

int menu_mort(JeuState* etat) {
    int selection = 0;
    char cle;
    int largeur_console, hauteur_console;

    etat->nb_vies--;

    if (etat->nb_vies <= 0) {
        effacer_ecran();
        printf("\nGame Over!\n");
        Sleep(1000);
        return 0;
    }

    while (1) {
        effacer_ecran();

        obtenir_taille_console(&largeur_console, &hauteur_console);

        int centre_x = (largeur_console - LARGEUR_MENU) / 2;
        int centre_y = (hauteur_console - 14) / 2;

        for (int i = 0; i < centre_y; i++) {
            printf("\n");
        }

        int centre_titre = (largeur_console - strlen("=== Vous avez perdu une vie! ===")) / 2;
        for (int i = 0; i < centre_titre; i++) {
            printf(" ");
        }
        printf("=== Vous avez perdu une vie! ===\n\n");

        int centre_vies = (largeur_console - strlen("Vies restantes: X")) / 2;
        for (int i = 0; i < centre_vies; i++) {
            printf(" ");
        }
        printf("Vies restantes: %d\n\n", etat->nb_vies);

        char* options[] = { "Recommencer", "Quitter" };
        int nb_options = 2;

        for (int i = 0; i < nb_options; i++) {
            printf("%*s+----------------------------------------+\n", centre_x, "");
            printf("%*s|", centre_x, "");

            if (i == selection) {
                int len = strlen(options[i]);
                int espaces = (LARGEUR_MENU - 4 - len - 4) / 2;
                for (int j = 0; j < espaces; j++) printf(" ");
                printf("[ %s ]", options[i]);
                for (int j = 0; j < LARGEUR_MENU - 4 - len - 4 - espaces; j++) printf(" ");
            }
            else {
                int len = strlen(options[i]);
                int espaces = (LARGEUR_MENU - 4 - len) / 2;
                for (int j = 0; j < espaces; j++) printf(" ");
                printf("%s", options[i]);
                for (int j = 0; j < LARGEUR_MENU - 4 - len - espaces; j++) printf(" ");
            }

            printf("|\n");
            printf("%*s+----------------------------------------+\n", centre_x, "");
        }

        cle = _getch();
        if (cle == -32 || cle == 0) {
            cle = _getch();
            if (cle == 72 && selection > 0) {
                selection--;
            }
            else if (cle == 80 && selection < nb_options - 1) {
                selection++;
            }
        }
        else if (cle == 27) {
            return 0;
        }
        else if (cle == 13) {
            if (selection == 0) {
                return 1;
            }
            else if (selection == 1) {
                return 0;
            }
        }

        Sleep(10);
    }
}

void respawn_joueur(JeuState* etat) {
    etat->camera_x = etat->position_respawn_x;
    etat->position_y = etat->position_respawn_y;
    etat->position_x = 0;
    etat->en_saut = 0;
    etat->phase_saut = 0;
}

void afficher_vies(JeuState* etat) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    COORD coord;
    coord.X = L_SCREEN - 15;
    coord.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    printf("Vies: %d", etat->nb_vies);

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), csbi.dwCursorPosition);
}