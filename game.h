#ifndef GAME_H
#define GAME_H

// Required includes for all parts of the game
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // For DWORD and GetTickCount()

// Constants
#define Size 20

// Global Variables (Declared as extern, actual definition is in game.c)
extern int playerRow;
extern int playerCol;
extern int keysCollected;
extern int gameOver;
extern const int totalKeys;
extern int exitSpawned;
extern int playerHealth;
extern DWORD startTime;

// Function Prototypes
void print_Instructions();
void Input_User(char str[Size][Size]);
// Returns remaining time for main loop check
int print_Temple(char str[Size][Size]); 
void Teleport(char str[Size][Size], char m);
void movePlayer(char str[Size][Size], char m);

#endif // GAME_H