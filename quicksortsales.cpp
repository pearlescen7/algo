/*****************************
 * Author: Uzay Uysal 		 *
 * Date: 05.12.2020			 *
 * Student Number: 150180039 *
*****************************/

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<chrono>
#include<iomanip>
using namespace std;

//Sale struct to hold the sale data.
struct sale
{
	string country;
	string i_type;
	string order_id;
	int	units_sold;
	float profit;
};

//Overloading operator< to use in quicksort as comparison criteria.
//If countries are the same return the one with the bigger profit.
//Otherwise do lexicographic comparison.
bool operator<(sale& l, sale& r)
{
	if (l.country == r.country)
	{
		return l.profit > r.profit;
	}
	else return l.country < r.country;
}

//Generic quicksort with pivot in the middle.
//Using templates makes it a bit more slower and we only sort sale type objects here so I didn't bother with it.
void QuickSort(int l, int h, vector<sale>& v)
{
	sale pivot = v[(l + h) / 2];  //Get a copy of the pivot so that we don't lose the value if it gets swapped.
	int i = l, j = h; 		      //Set the lower and upper boundaries since the values will be partially sorted in the calls after the first one.

	while(i <= j)				  //While we are in the desired partition.
	{
		while(v[i] < pivot) i++;  //Skip if the values on the left are smaller than pivot.
		while(pivot < v[j]) j--;  //Skip if the values on the right are bigger than pivot.
		if(i <= j)				  //Both indexes are out of place which means we can just swap them to fix the problem.
		{
			swap(v[i], v[j]);     //STL swap, best swap.
			i++;				  //Move i and j since we fixed those indexes as well.
			j--;
		}
	}
	
	if(l < j) QuickSort(l, j, v); //Do the same for the left partition.
	if(i < h) QuickSort(i, h, v); //Do the same for the right partition.
}


int main(int argc, char** argv){
	
	ifstream file;
	file.open("sales.txt");
	
	if (!file){
		cerr << "File cannot be opened!";
		exit(1);
		}
    
	int N = 0;

	if(argc == 1) N = 100; //you should read value of N from command line
	else N = stoi(argv[1]);
	string line;

	getline(file, line); //this is the header line

	vector<sale> allsales; //This vector holds all of the data needed.

	for(int i = 0; i<N; i++){
		sale newsale; //Create a sale object to store the data coming from the file.

		getline(file, line, '\t'); //country (string)
		newsale.country = line;

		getline(file, line, '\t'); //item type (string)
		newsale.i_type = line;

		getline(file, line, '\t'); //order id (string)
		newsale.order_id = line;

		file >> line; //units sold (integer)
		newsale.units_sold = stoi(line);

		file >> line; //total profit (float)
		newsale.profit = stof(line);

		getline(file, line, '\n'); //this is for reading the \n character into dummy variable.

		allsales.push_back(newsale); //Push the new data to the vector.
	}
	file.close();

	auto start = chrono::high_resolution_clock::now();	//Run the sort and calculate the elapsed time. 
	QuickSort(0, allsales.size()-1, allsales);	
	auto end = chrono::high_resolution_clock::now();

	chrono::duration<double>elapsed_time = end-start;
	std::cout << setprecision(10) <<  "Elapsed time: " << elapsed_time.count() << endl;

	ofstream outfile;
	outfile.open("sorted.txt", ios::out);  //Create a new file to put the sorted data.

	outfile << "Country	Item Type	Order ID	Units Sold	Total Profit\n";  //Header line.
	for(auto &i : allsales)
	{
		outfile << i.country << "\t" << i.i_type << "\t" << i.order_id << "\t" << i.units_sold << "\t" << i.profit << "\n";
	}  //Write the data.

	outfile.close();
	return 0;
}
