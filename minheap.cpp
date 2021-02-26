/****************************
* Author: Uzay Uysal        *
* Student Number: 150180039 *
****************************/

/*
min heap
- add element
- lower element
- extract min

taxis are removed after 100 ops

ops 
=> 0,01 decrease by p prob
=> add new by 1-p prob
*/

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <chrono>
#include <cmath>
#include <random>
#include <ratio>

//////////////////////////////////////////////////////////////////////////////////
//Simple min heap without templates.                                            //
//Uses a vector to cope with dynamic allocation and keep track of the size etc. //
//The indexing that is used with this implementation is as the following        //
//                                                                              //  
//                 PARENT(i/2)                                                  //
//                   //                                                         // 
//               NODE(i)                                                        //
//              //    \\                                                        //
//       LCHILD(2*i) RCHILD(2*i+1)                                              //
//////////////////////////////////////////////////////////////////////////////////
struct MinHeap
{
    std::vector<double> v;
    void insert_key(double);
    void decrease_key();
    void extract_min();
    void min_heapify(int index);
};

//Main function of min heap, min_heapify
//Works in a top-down manner, goes down from the element which it's called
void MinHeap::min_heapify(int index)
{
    if(v.size() <= 1) return;             //If the size is 1 or smaller there is nothing to fix
    int l = 2 * index, r = 2 * index + 1; //Calculate children indexes using the formula above
    int smallest = index;                 //Set the smallest as current index for now
    if(l < v.size() && v[l] < v[index]) smallest = l;     //Check if left child is smaller
    if(r < v.size() && v[r] < v[smallest]) smallest = r;  //Check if the right child is smaller

    if(smallest != index)                 //If the current node is bigger than one of its children
    {
        std::swap(v[smallest], v[index]); //Swap with that child
        min_heapify(smallest);            //Call heapify for the child to check the heap property
    }
}

//Inserts an element to the heap
void MinHeap::insert_key(double val)
{
    v.push_back(val);                           //Add the element to the end
    int index = v.size()-1;                     //Set the index to the last element which is the element that we just added
    while(index != 0 && v[index] < v[index/2])  //Check if we have to carry the element to the upper nodes
    {
        std::swap(v[index], v[index/2]);        //If we have to, swap with the parent
        index /= 2;                             //Set the new index as parent index, if we reach the root then it is done.
    }
}

//Decrease one of the node values by 0.01
void MinHeap::decrease_key()
{
    if(v.size() < 1) return;                    //Check if there is a key to decrease
    int index = rand() % v.size();              //Select a random node
    if(v[index] > 0)
    {
        v[index] -= 0.01;                           //Decrease the value
        while(index != 0 && v[index] < v[index/2])  //Do the same checks in the insert_heap function to carry the element to the right place.
        {
            std::swap(v[index], v[index/2]);
            index /= 2;
        }
    }
    else return;
}

//Extract the smallest element from the heap which is always the root
void MinHeap::extract_min()
{
    if(v.size() < 1) return;         //Check if there is anything to extract
    std::swap(v[v.size()-1], v[0]);  //Swap the root to the last element
    v.erase(v.end()-1);              //Delete the last element which is the element we just swapped
    min_heapify(0);                  //Call heapify for the root to fix the heap property
}

//Helper function to find distance between two points, I would prefer to use a lambda for this but the compiler standart is not specified.
double distance(double x1, double y1, double x2, double y2)
{ return sqrt((x1 - x2) * (x1 - x2) + (y1-y2) * (y1 - y2)); }

//Hotel location
double hotelx = 33.40819;
double hotely = 39.19001;

//Default parameters for the runs 
double p = 0.5;
long int m = 1000; 

int main(int argc, const char** argv) 
{
    std::ios_base::sync_with_stdio(false); //Cut off the stdio synchronization to make the output faster on terminal.
    std::cin.tie(NULL);

    srand(time(NULL));      //Set a seed for the rand() function
    MinHeap PQ;             //Declare the PQ using the min heap 
    std::vector<double> delvec;  //A vector to hold the extracted values

    m = std::stol(argv[1]); //Take arg 1 as m
    p = std::stod(argv[2]); //Take arg 2 as p 
    double randp;

    std::ifstream file;     //Input file stream to read from locations.txt
    std::string line;       //String variable to hold the current line

    file.open("locations.txt", std::ios_base::in); //Open the file in read mode
    std::getline(file, line);        //Read the dump line

    double x1, y1; //Vars to hold temporary long. lat.
    double dist;

    long int no_updt = 0, no_add = 0;  //Vars to hold number of updates and additions

    std::mt19937_64 rng;
    //Initialize the random number generator with time-dependent seed
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    //Initialize a uniform distribution between 0 and 1
    std::uniform_real_distribution<double> unif(0, 1);

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < m; i++)
    {
        if((i + 1) % 100 == 0)
        {
            if(PQ.v.size() != 0)
            {
                delvec.push_back(PQ.v[0]);
                PQ.extract_min();
                
            }
        }
        else 
        {
            randp = unif(rng); //Get random number between 0 and 1
            if(randp < p)                               //If in p probability decrease a random key
            {
                PQ.decrease_key();
                no_updt++;
            }
            else                                        //Else add a taxi from the file to the heap
            {   
                file >> x1;
                file >> y1;
                dist = distance(x1,y1,hotelx,hotely); //Calculate distance to hotel and push to heap
                PQ.insert_key(dist);
                no_add++;
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli>elapsed_time = end-start;

    for(auto i : delvec)
    {
        std::cout << "Distance of the taxi: "<< i << "\n";
    }
    
    std::cout << "Total number of taxi additions: " << no_add << std::endl;
    std::cout << "Total number of distance updates: " << no_updt << std::endl;
    std::cout << "Elapsed time: " << elapsed_time.count() << " ms"<< std::endl;

    return 0;
}