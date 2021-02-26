#include <iostream>
#include <stdlib.h> 
#include <time.h>
#define N 100


void merge(int* arr, int l, int m, int r)
{
	int size_l = m - l + 1;
	int size_r = r - m;
	
	int* temp_l = new int[size_l];
	int* temp_r = new int[size_r];
	
	for(int i = 0; i < size_l; i++)
	{
		temp_l[i] = arr[l + i];
	}
	for(int i = 0; i < size_r; i++)
	{
		temp_r[i] = arr[m + i + 1];
	}
	
	int i = 0, j = 0, k = l;
	
	while(i < size_l && j < size_r)
	{
		if(temp_l[i] <= temp_r[j])
		{
			arr[k] = temp_l[i];
			i++;
		}
		else
		{
			arr[k] = temp_r[j];
			j++;
		}
		k++;
	}
	
	while(i < size_l)
	{
		arr[k] = temp_l[i];
		i++;
		k++;
	}
	
	while(j < size_r)
	{
		arr[k] = temp_r[j];
		j++;
		k++;
	}
	
}

void MergeSort(int* arr, int l, int r)
{
	if(l < r)
	{
		int m = l + (r-l) / 2;
		MergeSort(arr, l, m);
		MergeSort(arr, m + 1, r);
		merge(arr, l, m, r);
	}
}

int main()
{
	int* arr = new int[N];
	for(int i = 0; i < N; i++)
	{
		arr[i] = i+1;
	}
	
	srand(time(NULL));
	
	std::cout << "=================================\nShuffled array\n=================================\n";
	for(int i = 0; i < N; i++)
	{	
		int temp_rand = rand() % N;
		int temp = arr[i];
		arr[i] = arr[temp_rand];
		arr[temp_rand] = temp;
		std::cout << arr[i] << " ";
	}
	MergeSort(arr, 0, N-1);
	
	std::cout << "\n=================================\nSorted array\n=================================\n";
	
	for(int i = 0; i < N; i++)
	{
		std::cout << arr[i] << " ";
	}

return 0;
}
