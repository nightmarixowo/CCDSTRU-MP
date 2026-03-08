#include "game.h"

int main(){
	Game g;
	int row, col;
	char *warning = NULL;
	int valid;

	initGame(&g);

	while(!over(&g)){
		clearScreen();
		showBoard(&g);
		if(warning){
			printf("\n  \033[1;33m%s\033[0m", warning);
			warning = NULL;
		}
		printf("\n%s's turn. Enter row and col (1-3): ", g.go ? "\033[1;31mR\033[0m" : "\033[1;34mB\033[0m");

		valid = 1;

		if(scanf("%d %d", &row, &col) != 2){
			warning = "Invalid input.";
			while(getchar() != '\n');
			valid = 0;
		}

		if(valid){
			Pos pos = {row - 1, col - 1};

			if(!inBounds(pos)){
				warning = "Out of bounds. Use 1-3.";
				valid = 0;
			}

			if(valid && g.start && (g.R[pos.row][pos.col] || g.B[pos.row][pos.col])){
				warning = "Position already occupied.";
				valid = 0;
			}

			if(valid && !g.start && !((g.go && g.R[pos.row][pos.col]) || (!g.go && g.B[pos.row][pos.col]))){
				warning = "You must select your own piece.";
				valid = 0;
			}

			if(valid){
				NextPlayerMove(&g, pos);
			}
		}
	}

	clearScreen();
	showBoard(&g);

	Result result = GameOver(&g);
	if(result == R_WIN)
		printf("\n\033[1;31mR wins!\033[0m\n");
	else if(result == B_WIN)
		printf("\n\033[1;34mB wins!\033[0m\n");
	else
		printf("\nDraw!\n");

	return 0;
}
