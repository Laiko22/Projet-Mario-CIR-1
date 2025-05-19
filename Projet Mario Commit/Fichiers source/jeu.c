#include "mario.h"
#include "menu.h"
#include "jeu.h"
#include "menuEchap.h"
#include "goomba.h"
#include "tableauScores.h"
#include "vie_mort.h"
#include "map_original.h"
#include <windows.h>
#include "musiqueM.h"

void init_jeu(JeuState* etat, char map[H_MAP][L_MAP]) {
	etat->position_y = 14;
	etat->camera_x = 0;
	etat->position_x = 0;
	etat->en_saut = 0;
	etat->phase_saut = 0;
	etat->quitter = 0;
	etat->direction = 0;
	etat->nbPiece = 0;
	init_vie_state(etat);
}


void deplacement_gauche(JeuState* etat, char map[H_MAP][L_MAP]) {
	if (etat->camera_x == 0) {
		if (etat->position_x > 0) {
			int nouvelle_pos_x = etat->position_x - VITESSE_DEPLACEMENT;
			if (!est_solide(nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	else if (etat->camera_x == L_MAP - L_SCREEN) {
		if (etat->position_x > 0) {
			int nouvelle_pos_x = etat->position_x - VITESSE_DEPLACEMENT;
			if (!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	else {
		int nouvelle_camera_x = etat->camera_x - VITESSE_DEPLACEMENT;
		if (nouvelle_camera_x >= 0) {
			int map_x = nouvelle_camera_x + etat->position_x;
			if (!est_solide(map_x, etat->position_y, map) &&
				!est_solide(map_x, etat->position_y - 1, map) &&
				!est_solide(map_x, etat->position_y - 2, map)) {
				etat->camera_x = nouvelle_camera_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	etat->direction = -1;
}

void deplacement_droite(JeuState* etat, char map[H_MAP][L_MAP]) {
	if (etat->camera_x == 0) {
		if (etat->position_x < L_SCREEN - 1) {
			int nouvelle_pos_x = etat->position_x + VITESSE_DEPLACEMENT;
			if (!est_solide(nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);

				if (etat->position_x > PERSO_X) {
					etat->camera_x += (etat->position_x - PERSO_X);
					etat->position_x = PERSO_X;
				}
			}
		}
	}
	else if (etat->camera_x == L_MAP - L_SCREEN) {
		if (etat->position_x < L_SCREEN - 1) {
			int nouvelle_pos_x = etat->position_x + VITESSE_DEPLACEMENT;
			if (!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	else {
		int nouvelle_camera_x = etat->camera_x + VITESSE_DEPLACEMENT;
		if (nouvelle_camera_x <= L_MAP - L_SCREEN) {
			int map_x = nouvelle_camera_x + etat->position_x;
			if (!est_solide(map_x, etat->position_y, map) &&
				!est_solide(map_x, etat->position_y - 1, map) &&
				!est_solide(map_x, etat->position_y - 2, map)) {
				etat->camera_x = nouvelle_camera_x;

				if (etat->camera_x > L_MAP - L_SCREEN) {
					etat->camera_x = L_MAP - L_SCREEN;
				}

				verifier_collision_pieces(etat, map);
			}
		}
	}
	etat->direction = 1;
}


void gestion_clavier(JeuState* etat, char map[H_MAP][L_MAP], int* nbPiece, int* score, Goomba* g, int nb_goombas) {

	etat->direction = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		etat->direction = -1;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		etat->direction = 1;
	}

	while (_kbhit()) {
		char touche = _getch();
		if (touche == 0 || touche == -32) {
			char fleche = _getch();

			if (fleche == 72 && !etat->en_saut) {
				int map_x = etat->camera_x + etat->position_x;
				if (est_solide(map_x, etat->position_y + 1, map)) {
					etat->en_saut = 1;
					etat->phase_saut = 0;
				}
			}
		}
		else if (touche == 32 && !etat->en_saut) {
			int map_x = etat->camera_x + etat->position_x;
			if (est_solide(map_x, etat->position_y + 1, map)) {
				etat->en_saut = 1;
				etat->phase_saut = 0;
			}
		}
		else if (touche == 27) {
			int resultat_menu = menuEchap(map);
			if (resultat_menu == 1) {
				etat->quitter = 1;
				return;
			}

			printf("\033[H");
			dessiner_avec_position_x(etat->camera_x, etat->position_y, etat->position_x, map, etat->en_saut, etat->phase_saut, *nbPiece, *score, g, nb_goombas);
			afficher_vies(etat);
		}
	}

	if (!etat->en_saut && (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState(VK_SPACE) & 0x8000)) {
		int map_x = etat->camera_x + etat->position_x;
		if (est_solide(map_x, etat->position_y + 1, map)) {
			etat->en_saut = 1;
			etat->phase_saut = 0;
		}
	}
}

int verifier_fin_niveau(JeuState* etat, char map[H_MAP][L_MAP]) {
	int map_x = etat->camera_x + etat->position_x;

	for (int x = 0; x < L_MAP; x++) {
		for (int y = 0; y < H_MAP; y++) {
			if (map[y][x] == '0') {
				if (map_x == x - 1 && etat->position_y == y) {
					return 1;
				}
				return 0;
			}
		}
	}
	return 0;
}

void afficher_felicitations(int nbPiece, int score) {
	sauvegarder_score_actuel(nbPiece, score);

	system("cls");

	int largeur = 50;
	int hauteur = 11;

	int pos_y = H_SCREEN / 2 - hauteur / 2;
	int pos_x = L_SCREEN / 2 - largeur / 2;

	char* messages[] = {
		"HERE WE GO !",
		"VOUS AVEZ FINI LE NIVEAU",
		"",
		"Appuyez sur une touche",
		"pour revenir au menu"
	};
	int nb_messages = sizeof(messages) / sizeof(messages[0]);

	printf("\033[%d;%dH", pos_y, pos_x);
	for (int i = 0; i < largeur; i++) {
		printf("*");
	}

	int ligne_message = 0;
	for (int i = 1; i < hauteur - 1; i++) {
		printf("\033[%d;%dH", pos_y + i, pos_x);
		printf("*");

		if (i >= 2 && i < 2 + nb_messages && ligne_message < nb_messages) {
			int longueur_msg = strlen(messages[ligne_message]);
			int padding = (largeur - 2 - longueur_msg) / 2;

			for (int j = 0; j < padding; j++) {
				printf(" ");
			}

			printf("%s", messages[ligne_message]);
			ligne_message++;

			for (int j = 0; j < largeur - 2 - padding - longueur_msg; j++) {
				printf(" ");
			}
		}
		else {
			for (int j = 0; j < largeur - 2; j++) {
				printf(" ");
			}
		}

		printf("*");
	}

	printf("\033[%d;%dH", pos_y + hauteur - 1, pos_x);
	for (int i = 0; i < largeur; i++) {
		printf("*");
	}

	_getch();
}


void mise_a_jour_jeu(JeuState* etat, char map[H_MAP][L_MAP], int* nbPiece, int* score, Goomba* g, int nb_goombas) {
	verifier_mort(etat, map, *nbPiece, *score, g, nb_goombas);

	if (verifier_fin_niveau(etat, map)) {
		afficher_felicitations(*nbPiece, *score);
		etat->quitter = 1;
		return;
	}

	if (etat->direction == -1) {
		deplacement_gauche(etat, map);
	}
	else if (etat->direction == 1) {
		deplacement_droite(etat, map);
	}


	if (etat->en_saut) {
		int map_x = etat->camera_x + etat->position_x;
		if (etat->phase_saut < HAUTEUR_SAUT) {
			if (!est_solide(map_x, etat->position_y - 1, map)) {
				etat->position_y--;
				verifier_collision_pieces(etat, map);
			}
			else {
				etat->phase_saut = HAUTEUR_SAUT;
			}
			etat->phase_saut++;
		}
		else if (etat->phase_saut < 2 * HAUTEUR_SAUT) {
			if (!est_solide(map_x, etat->position_y + 1, map)) {
				etat->position_y++;
				verifier_collision_pieces(etat, map);
			}
			else {
				etat->en_saut = 0;
			}
			etat->phase_saut++;
		}
		else {
			etat->en_saut = 0;
		}
	}
	else {
		int map_x = etat->camera_x + etat->position_x;
		if (!est_solide(map_x, etat->position_y + 1, map)) {
			etat->position_y++;
			verifier_collision_pieces(etat, map);
		}
	}

	*nbPiece = etat->nbPiece;
}

void verifier_mort(JeuState* etat, char map[H_MAP][L_MAP], int nbPiece, int score, Goomba* g, int nb_goombas) {
	int statut_mort = est_mort(etat, map, g, nb_goombas);

	if (statut_mort > 0) {
		int resultat = menu_mort(etat);

		if (resultat == 1) {
			respawn_joueur(etat);
			effacer_ecranE();
			dessiner(etat->camera_x, etat->position_y, map, etat->en_saut, etat->phase_saut, nbPiece, score, g, nb_goombas);
			afficher_vies(etat);
		}
		else {
			etat->quitter = 1;
		}
	}
}


void verifier_collision_pieces(JeuState* etat, char map[H_MAP][L_MAP]) {
	int map_x = etat->camera_x + etat->position_x;

	for (int partie = 0; partie < 3; partie++) {
		int pos_y = etat->position_y - partie;

		if (pos_y >= 0 && pos_y < H_MAP && map_x >= 0 && map_x < L_MAP) {
			if (map[pos_y][map_x] == 'o') {
				map[pos_y][map_x] = '.';
				etat->nbPiece++;
			}
		}
	}
}

void boucle_jeu(char map[H_MAP][L_MAP], int nbPiece, int score, Goomba* g, int nb_goombas) {
	reinitialiser_carte(map, map_original);
	JeuState etat;
	init_jeu(&etat, map);
	etat.nbPiece = nbPiece;

	int nbPiece_actuel = etat.nbPiece;
	int score_actuel = score;

	system("cls");

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	dessiner_avec_position_x(etat.camera_x, etat.position_y, etat.position_x, map, etat.en_saut, etat.phase_saut, nbPiece_actuel, score_actuel, g, nb_goombas);

	int derniere_mise_a_jour = GetTickCount64();
	int taux_rafraichissement = 70;

	continuer_musique = 1;

	afficher_vies(&etat);

	while (!etat.quitter) {
		score_actuel = score + (etat.camera_x);

		sauvegarder_score_actuel(nbPiece_actuel, score_actuel);

		scoreboard(nbPiece_actuel, score_actuel);

		gestion_clavier(&etat, map, &nbPiece_actuel, &score_actuel, g, nb_goombas);

		mise_a_jour_jeu(&etat, map, &nbPiece_actuel, &score_actuel, g, nb_goombas);

		for (int i = 0; i < nb_goombas; i++) {
			deplacer_goomba(&g[i], map);
		}

		int temps_actuel = GetTickCount64();
		if (temps_actuel - derniere_mise_a_jour >= taux_rafraichissement) {
			dessiner_avec_position_x(etat.camera_x, etat.position_y, etat.position_x, map, etat.en_saut, etat.phase_saut, nbPiece_actuel, score_actuel, g, nb_goombas);
			afficher_vies(&etat);
			derniere_mise_a_jour = temps_actuel;
		}

		Sleep(50);
	}

	saisir_nouveau_score(nbPiece_actuel, score_actuel);

	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}