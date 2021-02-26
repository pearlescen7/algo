#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
	while (1){
		char *morse[27] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..","/"};
		char input[50];
		char *output[50];

		printf("Enter the text you want to convert into morse code (Maximum size 49 characters): ");
		if (fgets(input, sizeof(input), stdin) == NULL)
	    	return 0;

		int i = 0;

		//Make all of the chars uppercase to compare with the array.
	    while(isalpha(input[i]) || isspace(input[i])) {
	    	char c = toupper(input[i]);
	    	if (isspace(input[i])){
	    		output[i++] = morse[26];
	      	} else{
	    		output[i++] = morse[c - 'A']; //Assign the according morse code to the output's pointers.
	    	}
	    }

	    printf("Morse code: \n");
	    
	    int n = 0;
	    while (n < i){
	      printf("%s", output[n++]);
	    }

	    putchar('\n');
	}

		return EXIT_SUCCESS;
}