#include <iostream>
#include <chrono>
#define N 1000000
#define lint long long int

int main()
{
	lint* arr = new lint[N];
    for(int i = 0; i < N; i++)
	{
		arr[i] = 0;
	}
    auto start = std::chrono::high_resolution_clock::now();
	
    for (lint i = 2; i < N; i++)
    {
        for (lint j = i * i; j < N; j+=i)
        {
            arr[j - 1] = 1;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    for (lint i = 1; i < N; i++)
    {
        if (arr[i - 1] == 0) std::cout << i << "\t";
    }

    std::cout << "Elapsed time in seconds : " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds";
    getchar();
}