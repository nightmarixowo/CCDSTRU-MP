#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Structs and Types
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
int cardinality(bool b[3][3]){
	int count = 0;
	int i, j;
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			if(b[i][j]) count++;
	return count;
}

bool inBounds(Pos pos){
	return pos.row >= 0 && pos.row < 3 && pos.col >= 0 && pos.col < 3;
}

// F = M - (R ∪ B), over ↔ (|F|=3 ∨ val≥20 ∨ (¬start ∧ (|R|>0∧|B|=0 ∨ |R|=0∧|B|>0)))
bool over(Game *g){
	int f = 0;
	int r = cardinality(g->R);
	int b = cardinality(g->B);
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			if(!g->R[i][j] && !g->B[i][j]) f++;

	return (f == 3 || g->val >= 20 ||
		(!g->start && ((r > 0 && b == 0) || (r == 0 && b > 0))));
}

void clearScreen(){
	printf("\033[2J\033[H");
}

// System Initialization
void initGame(Game *g){
	g->good = false;
	g->go = true;
	g->start = true;
	g->found = false;
	g->val = 0;
	memset(g->R, false, sizeof(g->R));
	memset(g->B, false, sizeof(g->B));
	memset(g->S, false, sizeof(g->S));
	memset(g->T, false, sizeof(g->T));
}

// Main Logic
void Remove(Game *g, Pos pos){
	if(g->go){
		g->R[pos.row][pos.col] = false;
	}
	if(!g->go){
		g->B[pos.row][pos.col] = false;
	}

	g->S[pos.row][pos.col] = false;
	g->T[pos.row][pos.col] = false;
}

void Expand(Game *g, Pos pos);

void Replace(Game *g, Pos pos){
	if(inBounds(pos)){
		g->found = false;

		// If go is true
		if(g->go && g->B[pos.row][pos.col]){
			g->B[pos.row][pos.col] = false;
			g->found = true;
		}
		if(g->go && g->R[pos.row][pos.col]){
			g->found = true;
		}
		if(g->go && !g->R[pos.row][pos.col]){
			g->R[pos.row][pos.col] = true;
		}

		// If go is false
		if(!g->go && g->R[pos.row][pos.col]){
			g->R[pos.row][pos.col] = false;
			g->found = true;
		}
		if(!g->go && g->B[pos.row][pos.col]){
			g->found = true;
		}
		if(!g->go && !g->B[pos.row][pos.col]){
			g->B[pos.row][pos.col] = true;
		}

		// If found
		if(g->found && !g->S[pos.row][pos.col]){
			g->S[pos.row][pos.col] = true;
			g->found = false;
		}
		else if(g->found && g->S[pos.row][pos.col] && !g->T[pos.row][pos.col]){
			g->T[pos.row][pos.col] = true;
			Expand(g, pos);
		}
	}
}

void Expand(Game *g, Pos pos){
	Pos u = {pos.row - 1, pos.col};
	Pos d = {pos.row + 1, pos.col};
	Pos k = {pos.row, pos.col - 1};
	Pos r = {pos.row, pos.col + 1};

	Remove(g, pos);

	if(g->go && inBounds(u)) Replace(g, u);
	if(!g->go && inBounds(d)) Replace(g, d);

	if(inBounds(k)) Replace(g, k);
	if(inBounds(r)) Replace(g, r);
}

void Update(Game *g, Pos pos){
	g->good = false;
	if(!g->S[pos.row][pos.col]){
		g->S[pos.row][pos.col] = true;
		g->good = !g->good;
	}
	if(!g->good && g->S[pos.row][pos.col] && !g->T[pos.row][pos.col]){
		g->T[pos.row][pos.col] = true;
		Expand(g, pos);
	}
}

void NextPlayerMove(Game *g, Pos pos){
	if(!over(g) && g->start && g->go){
		g->R[pos.row][pos.col] = true;
		g->S[pos.row][pos.col] = true;
		g->good = true;
	}

	if(!over(g) && g->start && !g->go){
		g->B[pos.row][pos.col] = true;
		g->S[pos.row][pos.col] = true;
		g->good = true;
	}

	if(!over(g) && !g->start && ((g->go && g->R[pos.row][pos.col]) || (!g->go && g->B[pos.row][pos.col]))){
		Update(g, pos);
		g->good = true;
	}

	if(g->start && cardinality(g->R) == 1 && cardinality(g->B) == 1){
		g->start = false;
	}

	if(!over(g) && g->good){
		g->good = !g->good;
		g->go = !g->go;
		g->val = g->val + 1;
	}
}

Result GameOver(Game *g){
	int r = cardinality(g->R);
	int b = cardinality(g->B);
	Result result = DRAW;

	if(over(g) && r > b) result = R_WIN;
	if(over(g) && r < b) result = B_WIN;

	return result;
}

// === RAYLIB UI ===
#define CELL 100
#define SIZE (CELL*3)

void drawBoard(Game *g){
	int cell=100;

	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			int x=j*cell, y=i*cell;

			DrawRectangleLines(x,y,cell,cell,WHITE);

			if(g->R[i][j] && g->S[i][j] && !g->T[i][j])
				DrawText("RR",x+30,y+30,30,RED);
			else if(g->B[i][j] && g->S[i][j] && !g->T[i][j])
				DrawText("BB",x+30,y+30,30,BLUE);
			else if(g->R[i][j])
				DrawText("R",x+35,y+30,30,RED);
			else if(g->B[i][j])
				DrawText("B",x+35,y+30,30,BLUE);
		}
	}

	DrawText(TextFormat("Turn: %s Move: %d",
		g->go?"R":"B", g->val), 10,310,20,WHITE);
}

// === MAIN ===
int main()
{
	InitWindow(SIZE, SIZE+40, "Game");
	SetTargetFPS(60);

	Game g;
	initGame(&g);

	while(!WindowShouldClose()){

		if(!over(&g) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			Vector2 m = GetMousePosition();
			int col = m.x / CELL;
			int row = m.y / CELL;

			Pos pos = {row,col};
			if(inBounds(pos))
				NextPlayerMove(&g,pos);
		}

		BeginDrawing();
		ClearBackground(BLACK);

		drawBoard(&g);

		if(over(&g)){
			Result r = GameOver(&g);
			const char *msg = "DRAW!";
			Color col = WHITE;

			if(r==R_WIN){ msg="R WINS!"; col=RED; }
			if(r==B_WIN){ msg="B WINS!"; col=BLUE; }

			DrawText(msg, SIZE/2-50, SIZE/2-10, 30, col);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}