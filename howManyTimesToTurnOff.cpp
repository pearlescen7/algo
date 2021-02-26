#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;


int main() {
    int n, k, temp, res = 1;
    cin >> n >> k;
    int t[n];
    for(int i = 0; i < n; i++)
    {
        cin >> temp;
        t[i] = temp;
    }
    priority_queue<int> q;
    for(int i = 1; i < n; i++)
    {
        q.push(t[i]-t[i-1]);
    }
    for(int i = 0; i < k-1; i++)
    {
        q.pop();
        res++;
    }
    while(!q.empty())
    {
        res += q.top();
        q.pop();
    }
    cout << res;
    return 0;
}