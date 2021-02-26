#include <bits/stdc++.h>

using namespace std;

// Complete the longestIncreasingSubsequence function below.
/* N^2 solution 
int longestIncreasingSubsequence(vector<int> arr) {
    int n = arr.size();
    int res = 0;
    int dp[n];
    for(int i = 0; i < n; i++)
    {
        dp[i] = 1;
        for(int j = 0; j < i; j++)
        {
            if(arr[i] > arr[j])
            {
                dp[i] = max(dp[i], dp[j] + 1);
                if(dp[i] > res) res = dp[i];
            }
        }
    }
    return res;
}
*/

int bSearch(vector<int> lis, int val)
{   
    int low = 0;
    int high = lis.size()-1;
    int mid;
    while(high != low)
    {
        mid = (high+low)/2;
        if(lis[mid] < val) low = mid + 1;
        else if (lis[mid] == val) return mid;
        else high = mid;
    }
    return low;
}

int longestIncreasingSubsequence(vector<int> arr) {
    int n = arr.size();
    vector<int> lis;
    lis.push_back(arr[0]);
    for(int i = 1; i < n; i++)
    {
        if(arr[i] > *(lis.end()-1))
        {
            lis.push_back(arr[i]);
        }
        else if (arr[i] == *(lis.end()-1)) {continue;}
        else
        {
            //binary search to get the best possible lis
            lis[bSearch(lis, arr[i])] = arr[i];
        }
    }
    for(int i = 0; i < lis.size(); i++)
    {
        cout << lis[i] << " ";
    }
    cout << lis.size();
    return lis.size();
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        int arr_item;
        cin >> arr_item;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        arr[i] = arr_item;
    }

    int result = longestIncreasingSubsequence(arr);

    fout << result << "\n";

    fout.close();

    return 0;
}