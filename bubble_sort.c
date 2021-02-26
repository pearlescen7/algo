#include <stdio.h> //printf, scanf
#include <stdlib.h> //EXIT_FAILURE, EXIT_SUCCESS


int* get_input(int n){

	int* arr = NULL;
	arr = malloc(n * sizeof(int));

	printf("Enter the numbers one by one.\n");
	for(int i = 0; i < n; i++){
		scanf("%d", (arr + i));
	}

	return arr;
}

void bubble_sort(int n, int* arr) {
   int i = 0, j = 0, tmp;
   for (i = 0; i < n; i++) {
       for (j = 0; j < n - i - 1; j++) {          
            if (*(arr + j) > *(arr + (j+1))) {
               tmp = *(arr + j);
               *(arr + j) = *(arr + (j+1));
               *(arr + (j+1)) = tmp;
           }
       }
   }
}

int main(){
	int n;
	int* inp_arr;
	
	printf("Size of array: ");
	scanf("%d", &n);
	inp_arr = get_input(n);		//Get the input
	printf("First array: [ ");		//Print out the first array
	for(int i = 0; i < n; i++){
		printf("%d ", *(inp_arr + i));
	}
	printf("]\n");
	bubble_sort(n, inp_arr);
	
	printf("Sorted array in ascending order: [ ");  //Print the result in ascending order
	for(int i = 0; i < n; i++){
		printf("%d ", *(inp_arr + i));
	}
	printf("]\n");
	
	printf("Sorted array in descending order: [ ");  //Print the result in descending order
	for(int i = (n - 1); i >= 0; i--){
		printf("%d ", *(inp_arr + i));
	}
	printf("]\n");
	system("pause");
	return EXIT_SUCCESS;
}