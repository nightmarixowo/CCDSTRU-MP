#include "game.h"

// System Variables & Initialization
bool good = false;
bool go = true;
bool start = true;
bool found = false;
int val = 0;
Board R, B, S, T;

// Helper Functions
int cardinality(Board b){
	int count = 0;
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			if(b[i][j]) count++;
	return count;
}

bool inBounds(Pos pos){
	return pos.row >= 0 && pos.row < 3 && pos.col >= 0 && pos.col < 3;
}

// F = M - (R ∪ B), over ↔ (|F|=3 ∨ val≥20 ∨ (¬start ∧ (|R|>0∧|B|=0 ∨ |R|=0∧|B|>0)))
bool over(){
	int f = 0;
	int r = cardinality(R);
	int b = cardinality(B);

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			if(!R[i][j] && !B[i][j]) f++;

	return (f == 3 || val >= 20 ||
		(!start && ((r > 0 && b == 0) || (r == 0 && b > 0))));
}

void clearScreen()
{
	printf("\033[2J\033[H");
}



// Main Logic
void Remove(Pos pos){
	if (go){
		R[pos.row][pos.col] = false;
	}
	if (!go){
		B[pos.row][pos.col] = false;
	}

	S[pos.row][pos.col] = false;
	T[pos.row][pos.col] = false;
}

void Replace(Pos pos){
	if (!inBounds(pos)) return;

	found = false;

	// If go is true
	if(go && B[pos.row][pos.col]){
		B[pos.row][pos.col] = false;
		found = true;
	}
	if(go && R[pos.row][pos.col]){
		found = true;
	}
	if(go && !R[pos.row][pos.col]){
		R[pos.row][pos.col] = true;
	}

	// If go is false
	if(!go && R[pos.row][pos.col]){
		R[pos.row][pos.col] = false;
		found = true;
	}
	if(!go && B[pos.row][pos.col]){
		found = true;
	}
	if(!go && !B[pos.row][pos.col]){
		B[pos.row][pos.col] = true;
	}

	// If found
	if(found && !S[pos.row][pos.col]){
		S[pos.row][pos.col] = true;
		found = false;
	}
	else if(found && S[pos.row][pos.col] && !T[pos.row][pos.col]){
		T[pos.row][pos.col] = true;
		Expand(pos);
	}
}

void Expand(Pos pos){
	Pos u = {pos.row - 1, pos.col};
	Pos d = {pos.row + 1, pos.col};
	Pos k = {pos.row, pos.col - 1};
	Pos r = {pos.row, pos.col + 1};

	Remove(pos);

	if (go && inBounds(u)) Replace(u);
	if (!go && inBounds(d)) Replace(d);

	if (inBounds(k)) Replace(k);
	if (inBounds(r)) Replace(r);
}

void Update(Pos pos){
	good = false;
	if(!S[pos.row][pos.col]){
		S[pos.row][pos.col] = true;
		good = !good;
	}
	if(!good && S[pos.row][pos.col] && !T[pos.row][pos.col]){
		T[pos.row][pos.col] = true;
		Expand(pos);
	}
}

void NextPlayerMove(Pos pos){
	if(!over() && start && go){
		R[pos.row][pos.col] = true;
		S[pos.row][pos.col] = true;
		good = true;
	}

	if(!over() && start && !go){
		B[pos.row][pos.col] = true;
		S[pos.row][pos.col] = true;
		good = true;
	}

	if(!over() && !start && ((go && R[pos.row][pos.col]) || (!go && B[pos.row][pos.col]))){
		Update(pos);
		good = true;
	}

	if(start && cardinality(R) == 1 && cardinality(B) == 1){
		start = false;
	}

	if(!over() && good){
		good = !good;
		go = !go;
		val = val + 1;
	}
}

Result GameOver(){

	int r = cardinality(R);
	int b = cardinality(B);

	if(over() && r > b) return R_WIN;
	if(over() && r < b) return B_WIN;
	else return DRAW;
}

void showBoard(){
	printf("\n    1   2   3\n");
	printf("  +---+---+---+\n");
	for(int i = 0; i < 3; i++){
		printf("%d |", i + 1);
		for(int j = 0; j < 3; j++){
			if(R[i][j])
				printf(" \033[1;31mR\033[0m |");
			else if(B[i][j])
				printf(" \033[1;34mB\033[0m |");
			else
				printf("   |");
		}
		printf("\n  +---+---+---+\n");
	}
	printf("\n  Turn: %s | Move: %d\n", go ? "\033[1;31mR\033[0m" : "\033[1;34mB\033[0m", val);
}
