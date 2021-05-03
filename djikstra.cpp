/****************************
* Author: Uzay Uysal        *
* Student Number: 150180039 *
****************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <sstream>
#include <stack>
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


void printMa(string, string, unordered_map<string, string>);
typedef pair<string, pair<string, int> > g_edge;
//typedef pair<unordered_map<string,int>, unordered_map<string, string> > s_dist;
//set<string> vertices;
//bool set_vertices = true;

struct Graph
{
    multimap<string, pair<string, int> > adj_list;
    void add_edge(string, string, int);
    void djikstra_spt(string); 

private:
    bool check_edge(g_edge);
};

void Graph::add_edge(string n1, string n2, int d)
{
    adj_list.insert({ n1, {n2, d}});
    adj_list.insert({ n2, {n1, d}});
}

bool Graph::check_edge(g_edge e)
{
    if(e.first[0] == 'E') return false;
    if(e.second.first[0] == 'E' && e.second.second < 5) return false;
    return true;
};



void Graph::djikstra_spt(string src)
{
    MinHeap h;
    unordered_map<string, int> dist;
    unordered_map<string, bool> flag;
    unordered_map<string, string> parent;
    set<string> seen;

    //Find unique nodes
    //Set starting node distance as 0 and push to heap, set others as inf.
    for(auto it = adj_list.begin(); it != adj_list.end(); it++)
    {
        if(seen.find(it->first) == seen.end())
        {
            if(it->first == src)
            {
                seen.insert(src);
                h.insert_key({src, 0});
                dist[src] = 0;
                flag[src] = false;
                parent[src] = "-";
            }
            else
            {
                seen.insert(it->first);
                dist[it->first] = INT_MAX;
                flag[it->first] = false;
                parent[it->first] = "-";
            }
        }
    }

    /*if(set_vertices)
    {
        vertices = seen;
        set_vertices = false;
    }*/

    seen.clear();

    while(h.v.size() > 0)
    {
        auto u_node = h.v[0];
        string u = u_node.first;
        h.extract_min();
        flag[u] = true;
        bool e_node = false;

        auto trange = adj_list.equal_range(u);
        for(; trange.first != trange.second; trange.first++)
        {
            g_edge curr = *trange.first;
            if(!check_edge(curr)){ e_node = true; break;}
        }

        if(!e_node)
        {
            auto range = adj_list.equal_range(u);
            for(; range.first != range.second; range.first++)
            {
                g_edge curr = *range.first;
                int weight = curr.second.second;
                string v = curr.second.first;
                if(flag[v] == false && (dist[v] > dist[u] + weight))
                {
                    dist[v] = dist[u] + weight;
                    h.insert_key({v, dist[v]});
                    parent[v] = u;
                }
            }
        }
    }

    /*cout << "Distance from " << src << endl;
    for(auto it : dist)
    {
        cout << it.first << ":" << it.second << ":" << parent[it.first] << endl;
    }*/
    printMa("Ma", "Mo", parent);
    cout << dist["Mo"] << endl;
}

void printMa(string dest, string src, unordered_map<string, string> parent)
{
    cout << dest << " ";
    stack<string> s;
    while(src != dest)
    {
        s.push(src);
        src = parent[src];
    }
    while(!s.empty())
    {
        cout << s.top() << " ";
        s.pop();
    }
}

/*void printMo(string dest, string src, unordered_map<string, string> parent)
{

    src = parent[src];
    while(src != dest)
    {
        cout << src << " ";
        src = parent[src];
    }
    cout << dest << " ";
}*/

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
    }

    /*for(auto i : g.adj_list)
    {
        cout << i.first << " " << i.second.first << " " << i.second.second << endl;
    }*/

    
    g.djikstra_spt("Ma");
    
    /*unordered_map<string, s_dist> s_path_list;
    vector<string> rest_list;
    s_path_list["Ma"] = g.djikstra_spt("Ma");
    for(auto it : vertices)
    {
        if(it[0] != 'R') continue;
        //s_path_list[it] = g.djikstra_spt(it);
        rest_list.push_back(it);
    }
    s_path_list["Mo"] = g.djikstra_spt("Mo");

    long long int best_cost = INT_MAX;
    int best_rest = -1;
    for(size_t i = 0; i < rest_list.size(); i++)
    {
        //cout << s_path_list["Ma"].first[rest_list[i]] << " " << s_path_list[rest_list[i]].first["Mo"];
        long long int curr_cost = (long long int)s_path_list["Ma"].first[rest_list[i]] + (long long int)s_path_list["Mo"].first[rest_list[i]];
        //cout << curr_cost << " " << rest_list[i] << endl;
        if(best_cost > curr_cost) { best_cost = curr_cost; best_rest = i; }
    }

    printMa("Ma", rest_list[best_rest], s_path_list["Ma"].second);
    printMo("Mo", rest_list[best_rest], s_path_list["Mo"].second);
    cout << best_cost << endl;*/

    return 0;
}