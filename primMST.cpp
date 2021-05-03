/****************************
* Author: Uzay Uysal        *
* Student Number: 150180039 *
****************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <climits>

using namespace std;

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
    std::vector<pair<string, int> > v;
    void insert_key(pair<string, int>);
    void update_key(string, int);
    void extract_min();
    void min_heapify(int index);
    bool in_heap(string);
};

//Main function of min heap, min_heapify
//Works in a top-down manner, goes down from the element which it's called
void MinHeap::min_heapify(int index)
{
    if(v.size() <= 1) return;             //If the size is 1 or smaller there is nothing to fix
    int l = 2 * index, r = 2 * index + 1; //Calculate children indexes using the formula above
    int smallest = index;                 //Set the smallest as current index for now
    if((size_t)l < v.size() && v[l].second < v[index].second) smallest = l;     //Check if left child is smaller
    if((size_t)r < v.size() && v[r].second < v[smallest].second) smallest = r;  //Check if the right child is smaller

    if(smallest != index)                 //If the current node is bigger than one of its children
    {
        std::swap(v[smallest], v[index]); //Swap with that child
        min_heapify(smallest);            //Call heapify for the child to check the heap property
    }
}

//Inserts an element to the heap
void MinHeap::insert_key(pair<string, int> val)
{
    v.push_back(val);                           //Add the element to the end
    int index = v.size()-1;                     //Set the index to the last element which is the element that we just added
    while(index != 0 && v[index].second < v[index/2].second)  //Check if we have to carry the element to the upper nodes
    {
        std::swap(v[index], v[index/2]);        //If we have to, swap with the parent
        index /= 2;                             //Set the new index as parent index, if we reach the root then it is done.
    }
}

//Update the node value
void MinHeap::update_key(string node, int val)
{
    if(v.size() < 1) return;                    //Check if there is a key to decrease
    int index;
    for(size_t i = 0; i < v.size(); i++)
    {
        if(v[i].first == node)
        {
            index = i; break;
        }
    }
    v[index].second = val;
    while(index != 0 && v[index].second < v[index/2].second)  //Do the same checks in the insert_heap function to carry the element to the right place.
    {
        std::swap(v[index], v[index/2]);
        index /= 2;
    }
}

//Extract the smallest element from the heap which is always the root
void MinHeap::extract_min()
{
    if(v.size() < 1) return;         //Check if there is anything to extract
    std::swap(v[v.size()-1], v[0]);  //Swap the root to the last element
    v.erase(v.end()-1);              //Delete the last element which is the element we just swapped
    min_heapify(0);                  //Call heapify for the root to fix the heap property
}

bool MinHeap::in_heap(string node)
{
    for(size_t i = 0; i < v.size(); i++)
    {
        if(v[i].first == node) return true;
    }
    return false;
}

typedef pair<string, pair<string, int> > g_edge;
multimap<string, pair<string, int> > input;

struct Graph
{
    multimap<string, pair<string, int> > adj_list;
    void add_edge(string, string, int);
    void prim_mst(); 

private:
    //g_edge update_nbor(string, set<string>&, MinHeap&, vector<pair<string, int> >&);
    bool check_edge(g_edge);
};

void Graph::add_edge(string n1, string n2, int d)
{
    adj_list.insert({ n1, {n2, d}});
    adj_list.insert({ n2, {n1, d}});
}

string sbest_c;

bool Graph::check_edge(g_edge e)
{
    string n1 = e.first, n2 = e.second.first;
    if(n1 == "Hipp" && n2[0] == 'B') return false; //Hipp connected to bas
    if(n2 == "Hipp" && n1[0] == 'B') return false;
    if(n1[0] == 'H' && n1[1] == 'p' && n2[0] == 'H' && n2[1] == 'p') return false; //hp connected to hp
    return true;
}

void Graph::prim_mst()
{
    MinHeap h;
    set<string> nMST;
    //              dest       source   w
    unordered_map<string, pair<string, int> > MST;

    for(auto it = adj_list.begin(); it != adj_list.end(); it++)
    {
        if(nMST.find(it->first) == nMST.end())
        {
            if(it->first == "GP")
            {
                h.insert_key( {it->first, 0} );
                MST.insert( {"GP", {"", 0}} );
            }
            else
            {   
                h.insert_key( {it->first, INT_MAX} );
                MST.insert( {it->first, {"", INT_MAX}} );

            }
            nMST.insert(it->first);
        }
    }
    nMST.clear();


    auto range = adj_list.equal_range("Hipp");

    for(; range.first != range.second; range.first++)
    {
        //source dest w
        g_edge curr = *range.first;
        if(curr.second.first == "GP")
        {
            MST["Hipp"] = {"GP", curr.second.second};
            h.update_key("Hipp", 0);
        }
    }

    range = adj_list.equal_range("GP");
    int best_c = INT_MAX;
    for(; range.first != range.second; range.first++)
    {
        //source dest w
        g_edge curr = *range.first;
        if((curr.second.first[0] == 'C') && (curr.second.second < best_c))
        {
            sbest_c = curr.second.first;
            best_c = curr.second.second;
            MST[sbest_c] = {"GP", curr.second.second};
            h.update_key(sbest_c, 0);
        }   
    }
    //closest church found

    /*for(int i = 1; i < h.v.size(); i++)
    {
        cout << h.v[i].first << " " << h.v[i].second << endl;
    }*/
    while(h.v.size() > 0)
    {
        /*for(int i = 0; i < h.v.size(); i++)
        {
            cout << h.v[i].first << " " << h.v[i].second << endl;
        }
        cout << "\n========" << endl;*/
        pair<string, int> temp = h.v[0];
        h.extract_min();
        auto range = adj_list.equal_range(temp.first);
        //range = adj_list.equal_range(temp.first);

        for(; range.first != range.second; range.first++)
        {
            //source dest w
            g_edge curr = *range.first;
            if(check_edge(curr) && h.in_heap(curr.second.first) && (curr.second.second < MST[curr.second.first].second))
            {
                //if(curr.first )
                MST[curr.second.first] = {curr.first, curr.second.second};           
                h.update_key(curr.second.first, curr.second.second);
                //cout << curr.first << " " << curr.second.first << " " << curr.second.second << endl; 
            }
        }
        //cout << "=======\n";
    }

    vector<g_edge> eMST;
    for(auto it : MST)
    {
        auto inrange = input.equal_range(it.first);
        bool infound = false;
        for(; inrange.first != inrange.second; inrange.first++)
        {
            if((inrange.first->first == it.first) && (inrange.first->second.first == it.second.first) && (inrange.first->second.second == it.second.second))
            {
                infound = true;
                break;
            }
        }
                    
        if(infound) eMST.push_back({ it.first, {it.second.first, it.second.second}});
             //fix order         source           dest             w 
        else eMST.push_back({ it.second.first, {it.first, it.second.second}});
    }

    sort(eMST.begin(), eMST.end(), [](const g_edge& lhs, const g_edge& rhs){return lhs.second.second < rhs.second.second;});

    int cost = 0;
    for(size_t i = 1; i < eMST.size(); i++)
    {
        cout << eMST[i].first << " " << eMST[i].second.first << " " << eMST[i].second.second << endl;
        cost += eMST[i].second.second;
    }
    cout << cost << endl;
}


int main(int argc, const char** argv) {

    string fname;
    cin >> fname;
    ifstream city_plan(fname);

    Graph g;
    string temp;
    while(getline(city_plan, temp))
    {
        stringstream ss(temp);
        string n1, n2; int d;
        getline(ss, n1, ',');
        getline(ss, n2, ',');
        ss >> d;
        g.add_edge(n1, n2, d);
        input.insert({n1, {n2, d}});
    }

    /*for(auto i : g.adj_list)
    {
        cout << i.first << " " << i.second.first << " " << i.second.second << endl;
    }*/

    g.prim_mst();

    return 0;
}