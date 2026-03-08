#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef bool Board[3][3];

typedef struct{
	int row,col;
} Pos;

// System Variables & Initialization

bool good = false;
bool go = true;
bool start = true;
bool found = false;
int val = 0;
Board R, B, S, T; 

// Remove Function
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

void Expand(Pos pos); 

void Replace(Pos pos){
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

bool inBounds(Pos pos){
	return pos.row >= 0 && pos.row < 3 && pos.col >= 0 && pos.col < 3;
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

int main(){

}
