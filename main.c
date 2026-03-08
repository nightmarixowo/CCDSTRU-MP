#include "game.h"

int main(){
	int row, col;

	while(!over()){
		showBoard();
		printf("\n%s's turn. Enter row and col (1-3): ", go ? "\033[1;31mR\033[0m" : "\033[1;34mB\033[0m");

		if(scanf("%d %d", &row, &col) != 2){
			printf("Invalid input.\n");
			while(getchar() != '\n');
			continue;
		}

		Pos pos = {row - 1, col - 1};

		if(!inBounds(pos)){
			printf("Out of bounds. Use 1-3.\n");
			continue;
		}

		int prev_val = val;
		NextPlayerMove(pos);
		if(val == prev_val)
			printf("Invalid move.\n");
	}

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
