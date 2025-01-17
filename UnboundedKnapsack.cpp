#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the unboundedKnapsack function below.
int unboundedKnapsack(int k, vector<int> arr) {
    int dp[k+1];
    for(int i = 0; i < k+1; i++) dp[i] = k;
    
    for(int i = 0; i < arr.size(); i++)
    {
        for(int j = arr[i]; j < k+1; j++)
        {
            dp[j] = min(dp[j], dp[j - arr[i]] - arr[i]);
        }
    }
    for(int i = 0; i < k+1; i++) cout << dp[i] << " ";
    return k - dp[k];
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for(int zed = 0; zed < t; zed++)
    {
        string nk_temp;
        getline(cin, nk_temp);

        vector<string> nk = split_string(nk_temp);

        int n = stoi(nk[0]);

        int k = stoi(nk[1]);

        string arr_temp_temp;
        getline(cin, arr_temp_temp);

        vector<string> arr_temp = split_string(arr_temp_temp);

        vector<int> arr(n);

        for (int i = 0; i < n; i++) {
            int arr_item = stoi(arr_temp[i]);

            arr[i] = arr_item;
        }

        int result = unboundedKnapsack(k, arr);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
