#include <stdio.h> //printf, scanf
#include <stdlib.h> //EXIT_FAILURE, EXIT_SUCCESS
#include <math.h>	//rand
#include <time.h>	//seed for rand
#define N 1000000 //number of iterations
int main(){
	clock_t begin = clock();
	srand(time(NULL));
	int strat_1 = 0; //We don't change the door.
	int strat_2 = 0; //We change the door.
	for(int i = 0; i < N; i++){
		int prize_door = rand() % 3 + 1;
		int selected_door = rand() % 3 + 1;
		if(prize_door == selected_door){
			strat_1++; //We win if the door is right.
		} else {
			strat_2++; //We win if the other door is right.
		}
	}
	clock_t end = clock();
	double win_ratio_1 = (double) strat_1 / N;
	double win_ratio_2 = (double) strat_2 / N;
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; //Execution time
	printf("Winning ratio when you don't change the door: %.3lf \n", win_ratio_1);
	printf("Winning ratio when you change the door: %.3lf \n", win_ratio_2);
	printf("Execution time: %.4lf \n", time_spent);
	
	return EXIT_SUCCESS;
}