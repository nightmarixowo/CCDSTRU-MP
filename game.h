#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdbool.h>

typedef bool Board[3][3];

typedef enum {
	R_WIN,
	B_WIN,
	DRAW
} Result;

typedef struct{
	int row,col;
} Pos;

// System Variables
extern bool good, go, start, found;
extern int val;
extern Board R, B, S, T;

// Functions
int cardinality(Board b);
bool inBounds(Pos pos);
void Remove(Pos pos);
void Replace(Pos pos);
void Expand(Pos pos);
bool over();
void showBoard();

#endif
