#ifndef MUSIQUE_H
#define MUSIQUE_H
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <process.h>  // Pour _beginthread et _endthread

#define REST 0
#define C4  262   // Do
#define CS4 277   // Do#
#define D4  294   // Ré
#define DS4 311   // Ré#
#define E4  330   // Mi
#define F4  349   // Fa
#define FS4 370   // Fa#
#define G4  392   // Sol
#define GS4 415   // Sol#
#define A4  440   // La
#define AS4 466   // La#
#define B4  494   // Si
#define C5  523   // Do (octave supérieure)
#define CS5 554   // Do#
#define D5  587   // Ré
#define DS5 622   // Ré#
#define E5  659   // Mi
#define F5  698   // Fa
#define FS5 740   // Fa#
#define G5  784   // Sol
#define GS5 831   // Sol#
#define A5  880   // La
#define AS5 932   // La#
#define B5  988   // Si
#define C6  1047  // Do (deux octaves au-dessus)
#define CS6 1109  // Do#
#define D6  1175  // Ré
#define DS6 1245  // Ré#
#define E6  1319  // Mi
#define F6  1397  // Fa
#define FS6 1480  // Fa#
#define G6  1568  // Sol

// Variable globale pour contrôler la lecture de la musique
extern int continuer_musique;  // MODIFIE

void playTone(int freq, int duration_ms);
void playMusic(void* arg);
void stopMusic(void);  // MODIFIE

#endif