#include "game.h"

int main(){
	int row, col;
	char *warning = NULL;

	while(!over()){
		clearScreen();
		showBoard();
		if(warning){
			printf("\n  \033[1;33m%s\033[0m", warning);
			warning = NULL;
		}
		printf("\n%s's turn. Enter row and col (1-3): ", go ? "\033[1;31mR\033[0m" : "\033[1;34mB\033[0m");

		if(scanf("%d %d", &row, &col) != 2){
			warning = "Invalid input.";
			while(getchar() != '\n');
			continue;
		}

		Pos pos = {row - 1, col - 1};

		if(!inBounds(pos)){
			warning = "Out of bounds. Use 1-3.";
			continue;
		}

		if(start && (R[pos.row][pos.col] || B[pos.row][pos.col])){
			warning = "Position already occupied.";
			continue;
		}

		if(!start && !((go && R[pos.row][pos.col]) || (!go && B[pos.row][pos.col]))){
			warning = "You must select your own piece.";
			continue;
		}

		NextPlayerMove(pos);
	}

	clearScreen();
	showBoard();

	Result result = GameOver();
	if(result == R_WIN)
		printf("\n\033[1;31mR wins!\033[0m\n");
	else if(result == B_WIN)
		printf("\n\033[1;34mB wins!\033[0m\n");
	else
		printf("\nDraw!\n");

	return 0;
}
