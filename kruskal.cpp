#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'kruskals' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts WEIGHTED_INTEGER_GRAPH g as parameter.
 */

/*
 * For the weighted graph, <name>:
 *
 * 1. The number of nodes is <name>_nodes.
 * 2. The number of edges is <name>_edges.
 * 3. An edge exists between <name>_from[i] and <name>_to[i]. The weight of the edge is <name>_weight[i].
 *
 */

int kruskals(int g_nodes, vector<int> g_from, vector<int> g_to, vector<int> g_weight) {
    vector<vector<int> > edges;
    for(int i = 0; i < g_from.size(); i++)
    {
        vector<int> edge;
        edge.push_back(g_from[i]);
        edge.push_back(g_to[i]);
        edge.push_back(g_weight[i]);
        edges.push_back(edge);
    }
    sort(edges.begin(), edges.end(), [](const vector<int>& lhs, const vector<int>& rhs) -> bool 
    { 
        if(lhs[2] == rhs[2])
        {
            return ((lhs[0] + lhs[1]) < (rhs[0] + rhs[1]));
        } 
        else return lhs[2] < rhs[2];
    });
    
    map<int, int> m;
    int cost = 0;
    int i = 0;
    int edges_added = 0;
    while(edges_added != g_nodes-1)
    {
        auto first_exists = m.find(edges[i][0]);
        auto second_exists = m.find(edges[i][1]);
        if((first_exists == m.end()) && (second_exists == m.end()))
        {
            m.insert({ edges[i][0], edges[i][0] } );
            m.insert({ edges[i][1], edges[i][0] } );
            cost += edges[i][2];
            edges_added++;
            cout << edges[i][0] << " " << edges[i][1] << " " << edges[i][2] << endl;     
        }
        else if ((first_exists != m.end()) && (second_exists == m.end()))
        {
            int first_parent = m[edges[i][0]];
            m.insert({ edges[i][1],  first_parent} );
            cost += edges[i][2];
            edges_added++;
            cout << edges[i][0] << " " << edges[i][1] << " " << edges[i][2] << endl;     
        }
        else if ((first_exists == m.end()) && (second_exists != m.end()))
        {
            int second_parent = m[edges[i][1]];
            m.insert({ edges[i][0], second_parent } );
            cost += edges[i][2];
            edges_added++;
            cout << edges[i][0] << " " << edges[i][1] << " " << edges[i][2] << endl;     
        }
        else
        {
            int first_parent = m[edges[i][0]];
            int second_parent = m[edges[i][1]];
            if (first_parent != second_parent)
            {
                for(auto mit = m.begin(); mit != m.end(); mit++)
                {
                    if(mit->second == second_parent)
                    {
                        mit->second = first_parent;
                    }
                }
                cost += edges[i][2];
                edges_added++;
                cout << edges[i][0] << " " << edges[i][1] << " " << edges[i][2] << endl;
            }
        }
        i++;
    }
    
    /*for(auto mit = m.begin(); mit != m.end(); mit++)
    {
        cout << mit->first << " " << mit->second << endl;
    }*/
    
    return cost;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string g_nodes_edges_temp;
    getline(cin, g_nodes_edges_temp);

    vector<string> g_nodes_edges = split(rtrim(g_nodes_edges_temp));

    int g_nodes = stoi(g_nodes_edges[0]);
    int g_edges = stoi(g_nodes_edges[1]);

    vector<int> g_from(g_edges);
    vector<int> g_to(g_edges);
    vector<int> g_weight(g_edges);

    for (int i = 0; i < g_edges; i++) {
        string g_from_to_weight_temp;
        getline(cin, g_from_to_weight_temp);

        vector<string> g_from_to_weight = split(rtrim(g_from_to_weight_temp));

        int g_from_temp = stoi(g_from_to_weight[0]);
        int g_to_temp = stoi(g_from_to_weight[1]);
        int g_weight_temp = stoi(g_from_to_weight[2]);

        g_from[i] = g_from_temp;
        g_to[i] = g_to_temp;
        g_weight[i] = g_weight_temp;
    }

    int res = kruskals(g_nodes, g_from, g_to, g_weight);

    fout << res << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
