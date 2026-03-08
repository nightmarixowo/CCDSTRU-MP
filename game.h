#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdbool.h>

typedef struct{
	int row, col;
} Pos;

typedef enum {
	R_WIN,
	B_WIN,
	DRAW
} Result;

typedef struct{
	bool good, go, start, found;
	int val;
	bool R[3][3], B[3][3], S[3][3], T[3][3];
} Game;

// Helper Functions
int cardinality(bool b[3][3]);
bool inBounds(Pos pos);
bool over(Game *g);

// Game Logic
void initGame(Game *g);
void Remove(Game *g, Pos pos);
void Replace(Game *g, Pos pos);
void Expand(Game *g, Pos pos);
void Update(Game *g, Pos pos);
void NextPlayerMove(Game *g, Pos pos);
Result GameOver(Game *g);

// Display
void clearScreen();
void showBoard(Game *g);

#endif
