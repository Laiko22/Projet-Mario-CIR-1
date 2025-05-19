#include "tbScore.h"
#include "tableauScores.h"
void scoreboard(int nbPiece, int score) {
    int score_length = 0;
    int temp_score = score;
    do {
        score_length++;
        temp_score /= 10;
    } while (temp_score > 0);
    if (score_length < 5) {
        score_length = 5;
    }
    int width = 34;
    if (score_length > 5) {
        width += (score_length - 5);
    }
    printf("\033[1;1H");
    for (int i = 0; i < width; i++) {
        printf("#");
    }
    printf("\n");
    printf("#");
    for (int i = 0; i < width - 2; i++) {
        printf(" ");
    }
    printf("#\n");
    printf("# Pieces : %3d     Score : %*d #\n", nbPiece, score_length, score);
    printf("#");
    for (int i = 0; i < width - 2; i++) {
        printf(" ");
    }
    printf("#\n");
    for (int i = 0; i < width; i++) {
        printf("#");
    }
    printf("\n");
    sauvegarder_score_actuel(nbPiece, score);
}