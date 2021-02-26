#include <string>

using namespace std;

class Solution {
public:
    int myAtoi(string s)
    {   
        int i=0;
        long int n=s.length();

        while(isspace(s[i])) i++;

        bool isneg=false;
        long long int num=0;

        if(s[i]=='-' || s[i]=='+'){
            if(s[i]=='-') isneg=true;

            i++;
        }


        while(i!=n && isdigit(s[i]))
        {
            char ch=s[i];
            int dig=ch-'0';
            num=num*10+dig;

            if(num>INT_MAX)
            {
                if((isneg)) return INT_MIN;
                else return INT_MAX;
            }
            i++;
        }

        if(isneg) return -num;
        else return num;
    }
};