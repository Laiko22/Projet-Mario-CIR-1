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

void obtenir_taille_console(int* largeur, int* hauteur) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	*largeur = info.srWindow.Right - info.srWindow.Left + 1;
	*hauteur = info.srWindow.Bottom - info.srWindow.Top + 1;
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
		int largeur_console, hauteur_console;

		obtenir_taille_console(&largeur_console, &hauteur_console);

		int centre_x = (largeur_console - LARGEUR_MENU) / 2;

		int lignes_par_option = 4;
		int total_lignes_menu = nb_options * lignes_par_option;
		int centre_y = (hauteur_console - total_lignes_menu) / 2;

		for (int i = 0; i < centre_y; i++) printf("\n");

		for (int i = 0; i < nb_options; i++) {
			printf("%*s+----------------------------------------+\n", centre_x, "");
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
			printf("%*s+----------------------------------------+\n", centre_x, "");
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
	int largeur_console, hauteur_console;

	while (1) {
		effacer_ecran();


		obtenir_taille_console(&largeur_console, &hauteur_console);


		int centre_x = (largeur_console - LARGEUR_MENU) / 2;


		int lignes_par_option = 4;
		int total_lignes = NB_PERSONNAGES * lignes_par_option;

		int centre_y = (hauteur_console - total_lignes) / 2;

		for (int i = 0; i < centre_y; i++) {
			printf("\n");
		}

		int centre_titre = (largeur_console - strlen("=== Choix du personnage ===")) / 2;
		for (int i = 0; i < centre_titre; i++) {
			printf(" ");
		}
		printf("=== Choix du personnage ===\n\n");

		for (int i = 0; i < NB_PERSONNAGES; i++) {
			printf("%*s+----------------------------------------+\n", centre_x, "");
			printf("%*s|", centre_x, "");

			if (i == selection) {
				int len = strlen(persos[i]);
				int espaces = (LARGEUR_MENU - 4 - len - 4) / 2;
				for (int j = 0; j < espaces; j++) printf(" ");
				printf("[ %s ]", persos[i]);
				for (int j = 0; j < LARGEUR_MENU - 4 - len - 4 - espaces; j++) printf(" ");
			}
			else {
				int len = strlen(persos[i]);
				int espaces = (LARGEUR_MENU - 4 - len) / 2;
				for (int j = 0; j < espaces; j++) printf(" ");
				printf("%s", persos[i]);
				for (int j = 0; j < LARGEUR_MENU - 4 - len - espaces; j++) printf(" ");
			}

			printf("|\n");
			printf("%*s+----------------------------------------+\n", centre_x, "");
		}


		cle = _getch();
		if (cle == -32 || cle == 0) {
			cle = _getch();
			if (cle == 72 && selection > 0) selection--;
			else if (cle == 80 && selection < NB_PERSONNAGES - 1) selection++;
		}
		else if (cle == 27) {
			return; // Échap
		}
		else if (cle == 13) {
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