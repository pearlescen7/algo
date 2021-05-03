#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	int result = 0;
	result = fork();

	for(int i = 0; i < 2; i++)
	{
		if(result != 0)
		{
			result = fork();
		}
		if (result == 0)
		{
			printf("Child process %d. %d \n", i, getpid());
		}
		else
		{
			printf("Parent process (i=%d). %d \n", i, getpid());
		}
	}
	return 0; 
}


