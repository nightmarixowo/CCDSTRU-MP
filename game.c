#include "game.h"
#include <string.h>

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

// Display
void showBoard(Game *g){
	printf("\n     1   2   3\n");
	printf("   ╔═══╦═══╦═══╗\n");
	int i, j;
	for(i = 0; i < 3; i++){
		printf(" %d ║", i + 1);
		for(j = 0; j < 3; j++){
			if(g->R[i][j] && g->S[i][j] && !g->T[i][j])
				printf("\033[1;31mR\033[1;33m*\033[0m ║");
			else if(g->B[i][j] && g->S[i][j] && !g->T[i][j])
				printf("\033[1;34mB\033[1;33m*\033[0m ║");
			else if(g->R[i][j])
				printf(" \033[1;31mR\033[0m ║");
			else if(g->B[i][j])
				printf(" \033[1;34mB\033[0m ║");
			else
				printf("   ║");
		}
		if(i < 2)
			printf("\n   ╠═══╬═══╬═══╣\n");
		else
			printf("\n   ╚═══╩═══╩═══╝\n");
	}
	printf("\n   Turn: %s  │  Move: %d\n", g->go ? "\033[1;31mR\033[0m" : "\033[1;34mB\033[0m", g->val);
}
