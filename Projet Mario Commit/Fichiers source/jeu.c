#include "mario.h"
#include "menu.h"
#include "jeu.h"
#include "menuEchap.h"
#include "goomba.h"

void init_jeu(JeuState* etat, char map[H_MAP][L_MAP]) {
	etat->position_y = 14;
	etat->camera_x = 0;
	etat->position_x = 0;  // Position initiale du personnage � l'�cran (centre)
	etat->en_saut = 0;
	etat->phase_saut = 0;
	etat->quitter = 0;
	etat->direction = 0;
	init_vie_state(etat);
}

void deplacement_gauche(JeuState* etat, char map[H_MAP][L_MAP]) {
	// Si nous sommes au bord gauche de la carte et que le personnage n'est pas au bord gauche de l'�cran
	if (etat->camera_x == 0) {
		// Mode d�placement personnage (bord gauche)
		if (etat->position_x > 0) {
			int nouvelle_pos_x = etat->position_x - VITESSE_DEPLACEMENT;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	// Si nous sommes au bord droit de la carte
	else if (etat->camera_x == L_MAP - L_SCREEN) {
		// Mode d�placement personnage (bord droit)
		if (etat->position_x > 0) {
			int nouvelle_pos_x = etat->position_x - VITESSE_DEPLACEMENT;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	// Si nous sommes au milieu de la carte
	else {
		// Mode d�placement cam�ra
		int nouvelle_camera_x = etat->camera_x - VITESSE_DEPLACEMENT;
		if (nouvelle_camera_x >= 0) {
			int map_x = nouvelle_camera_x + etat->position_x;
			// V�rifier si le personnage ne va pas traverser un mur
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
	// Si nous sommes au bord gauche de la carte
	if (etat->camera_x == 0) {
		// Mode d�placement personnage (bord gauche)
		if (etat->position_x < L_SCREEN - 1) {
			int nouvelle_pos_x = etat->position_x + VITESSE_DEPLACEMENT;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);

				// Transition vers le mode cam�ra si on d�passe le centre
				if (etat->position_x > PERSO_X) {
					etat->camera_x += (etat->position_x - PERSO_X);
					etat->position_x = PERSO_X;
				}
			}
		}
	}
	// Si nous sommes au bord droit de la carte
	else if (etat->camera_x == L_MAP - L_SCREEN) {
		// Mode d�placement personnage (bord droit)
		if (etat->position_x < L_SCREEN - 1) {
			int nouvelle_pos_x = etat->position_x + VITESSE_DEPLACEMENT;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	// Si nous sommes au milieu de la carte
	else {
		// Mode d�placement cam�ra
		int nouvelle_camera_x = etat->camera_x + VITESSE_DEPLACEMENT;
		if (nouvelle_camera_x <= L_MAP - L_SCREEN) {
			int map_x = nouvelle_camera_x + etat->position_x;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(map_x, etat->position_y, map) &&
				!est_solide(map_x, etat->position_y - 1, map) &&
				!est_solide(map_x, etat->position_y - 2, map)) {
				etat->camera_x = nouvelle_camera_x;

				// Si on atteint le bord droit de la carte, ajuster la position
				if (etat->camera_x > L_MAP - L_SCREEN) {
					etat->camera_x = L_MAP - L_SCREEN;
				}

				verifier_collision_pieces(etat, map);
			}
		}
	}
	etat->direction = 1;
}


void gestion_clavier(JeuState* etat, char map[H_MAP][L_MAP], int nbPiece, int score, Goomba g) {

	if (_kbhit()) {
		char touche = _getch();
		if (touche == 0 || touche == -32) {
			char fleche = _getch();

			// Fl�che gauche
			if (fleche == 75) {
				deplacement_gauche(etat, map);//MODIF
				etat->direction = -1;
			}

			// Fl�che droite
			if (fleche == 77) {
				deplacement_droite(etat, map);//MODIF
				etat->direction = 1;
			}

			// Fl�che haut ou barre d'espace (pour sauter)
			if ((fleche == 72 || touche == 32) && !etat->en_saut) {
				int map_x = etat->camera_x + etat->position_x;
				if (est_solide(map_x, etat->position_y + 1, map)) {
					etat->en_saut = 1;
					etat->phase_saut = 0;
				}
			}
		}
		// Barre d'espace (pour sauter �galement)
		else if (touche == 32 && !etat->en_saut) {
			int map_x = etat->camera_x + etat->position_x;
			if (est_solide(map_x, etat->position_y + 1, map)) {
				etat->en_saut = 1;
				etat->phase_saut = 0;
			}
		}
		// Touche �chap (pour acc�der au menu pause)
		else if (touche == 27) {
			int resultat_menu = menuEchap(map);
			if (resultat_menu == 1) {
				etat->quitter = 1; // Quitter le jeu et retourner au menu principal
				return; // Sortir imm�diatement de la fonction
			}

			printf("\033[H"); // D�placer le curseur au d�but sans effacer
			// Redessiner tout l'�cran
			dessiner_avec_position_x(etat->camera_x, etat->position_y, etat->position_x, map, etat->en_saut, etat->phase_saut, nbPiece, score, g);
			afficher_vies(etat);
		}
	}
}
void mise_a_jour_jeu(JeuState* etat, char map[H_MAP][L_MAP], int nbPiece, int score, Goomba g) {
	verifier_mort(etat, map, nbPiece, score, g);


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
		// Appliquer la gravit�
		int map_x = etat->camera_x + etat->position_x;
		if (!est_solide(map_x, etat->position_y + 1, map)) {
			etat->position_y++;
			verifier_collision_pieces(etat, map);
		}
	}
}

void verifier_mort(JeuState* etat, char map[H_MAP][L_MAP], int nbPiece, int score, Goomba g) { // ** TOUTE LA FONCTION EST NOUVELLE AUSSI **
	int statut_mort = est_mort(etat, map);

	if (statut_mort > 0) {
		// Afficher le menu de mort et g�rer la r�ponse
		int resultat = menu_mort(etat);

		if (resultat == 1) {
			// Continuer en respawnant le joueur
			respawn_joueur(etat);
			// Redessiner l'�cran du jeu
			effacer_ecranE();
			dessiner(etat->camera_x, etat->position_y, map, etat->en_saut, etat->phase_saut, nbPiece, score, g);
			afficher_vies(etat);
		}
		else {
			// Quitter et retourner au menu principal
			etat->quitter = 1;
		}
	}
}


void verifier_collision_pieces(JeuState* etat, char map[H_MAP][L_MAP]) {
	int map_x = etat->camera_x + etat->position_x;

	// V�rifier pour les trois parties du corps du personnage (t�te, torse, pieds)
	for (int partie = 0; partie < 3; partie++) {
		int pos_y = etat->position_y - partie;

		// Si la position est valide
		if (pos_y >= 0 && pos_y < H_MAP && map_x >= 0 && map_x < L_MAP) {
			// Si on trouve une pi�ce � cette position
			if (map[pos_y][map_x] == 'o') {
				// Collecte de la pi�ce
				map[pos_y][map_x] = '.';  // Remplacer la pi�ce par un espace vide
				etat->nbPiece++;  // Incr�menter le compteur de pi�ces
			}
		}
	}
}

void boucle_jeu(char map[H_MAP][L_MAP], int nbPiece, int score, Goomba g) {
	JeuState etat;
	init_jeu(&etat, map);
	etat.nbPiece = nbPiece; // Initialiser avec la valeur fournie

	// Effacer compl�tement l'�cran une fois au d�but
	system("cls");

	// Configuration pour r�duire le clignotement
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE; // Masquer le curseur
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	// Initialiser l'affichage
	dessiner_avec_position_x(etat.camera_x, etat.position_y, etat.position_x, map, etat.en_saut, etat.phase_saut, etat.nbPiece, score, g);

	int score_actuel = score;
	int derniere_mise_a_jour = GetTickCount();
	int taux_rafraichissement = 70; // 70ms entre chaque rafra�chissement

	afficher_vies(&etat);

	while (!etat.quitter) {
		// Mettre � jour le score bas� sur la progression
		score_actuel = score + (etat.camera_x);

		// G�rer les entr�es clavier
		gestion_clavier(&etat, map, etat.nbPiece, score_actuel, g);

		// Mettre � jour la position du personnage et des ennemis
		mise_a_jour_jeu(&etat, map, nbPiece, score, g);

		// Mettre � jour la position du Goomba
		deplacer_goomba(&g, map);

		// Redessiner l'�cran � un taux de rafra�chissement constant
		int temps_actuel = GetTickCount();
		if (temps_actuel - derniere_mise_a_jour >= taux_rafraichissement) {
			dessiner_avec_position_x(etat.camera_x, etat.position_y, etat.position_x, map, etat.en_saut, etat.phase_saut, etat.nbPiece, score_actuel, g);
			afficher_vies(&etat);
			derniere_mise_a_jour = temps_actuel;
		}

		// Petite pause pour �conomiser le CPU
		Sleep(50);
	}

	// Restaurer la visibilit� du curseur � la fin
	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}
