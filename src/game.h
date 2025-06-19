#ifndef GAME_H
#define GAME_H

#include <MD_MAX72xx.h>
#include <Arduino.h>

// External interrupt variables from main.cpp
extern volatile bool buttonPressed;

extern int blockPos;
extern int blockWidth;
extern int stackPos[];
extern int stackWidth[];
extern int stackHeight;
extern int direction;
extern int speed;
extern int level;
extern bool gameRunning;
extern MD_MAX72XX mx;

void moveBlock();
void handleButtonPress(); // New interrupt-based function
void increaseDifficulty();
void drawGame();
void gameOver();
void resetGame();

#endif // GAME_H
