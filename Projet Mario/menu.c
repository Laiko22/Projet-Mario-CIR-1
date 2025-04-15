#include "menu.h"

char obtenir_touche() {
	return _kbhit() ? _getch() : 0;
}

void pause_console() {
	printf("\nAppuyez sur une touche pour continuer...\n");
	obtenir_touche();
}

void effacer_ecran() {
	system("cls");
}

void afficher_menu_principal(int selection) {
	char* menu[] = { "Nouvelle partie", "Sauvegardes", "Tableau des scores", "Quitter" };
	int nb_options = 4;

	effacer_ecran();
	for (int i = 0; i < nb_options; i++) {
		if (i == selection) {
			printf("%*s+------------------------------------------+\n", CENTRE_X, "");
			printf("%*s|              [ %s ]              |\n", CENTRE_X, menu[i]);
			printf("%*s+------------------------------------------+\n", CENTRE_X, "");
		}
		else {
			printf("%*s+------------------------------------------+\n", CENTRE_X, "");
			printf("%*s|                %s                |\n", CENTRE_X, menu[i]);
			printf("%*s+------------------------------------------+\n", CENTRE_X, "");
		}
	}
}

int gestion_touches_fleche(int* selection, int max) {
	char cle = obtenir_touche();
	if (cle == -32 || cle == 0) {
		cle = obtenir_touche();
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

void choix_pseudo(char* pseudo) {
	effacer_ecran();
	printf("=== Choix du pseudo ===\n\n");
	printf("Entrez votre pseudo : ");
	scanf_s("%49s", pseudo, (unsigned)_countof(pseudo));
	pause_console();
}

void choix_personnage(char* perso) {
	char* persos[NB_PERSONNAGES] = { "Mario", "Luigi", "Toad", "Yoshi" };
	int selection = 0;
	while (1) {
		effacer_ecran();
		printf("=== Choix du personnage ===\n\n");

		for (int i = 0; i < NB_PERSONNAGES; i++) {
			if (i == selection) {
				printf("%*s+------------------------------------------+\n", CENTRE_X, "");
				printf("%*s|              [ %s ]              |\n", CENTRE_X, persos[i]);
				printf("%*s+------------------------------------------+\n", CENTRE_X, "");
			}
			else {
				printf("%*s+------------------------------------------+\n", CENTRE_X, "");
				printf("%*s|                %s                |\n", CENTRE_X, persos[i]);
				printf("%*s+------------------------------------------+\n", CENTRE_X, "");
			}
		}

		int resultat = gestion_touches_fleche(&selection, NB_PERSONNAGES - 1);
		if (resultat == -1) return;

		char cle = obtenir_touche();
		if (cle == 13) {
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
	printf("Nouvelle partie lancée avec %s (%s) !\n", pseudo, perso);
	pause_console();
}