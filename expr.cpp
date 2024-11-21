#include<iostream>
#include<string>
using namespace std;


void advance(int&i)
{
    while(1)
    {
        int t;
        cin>>t;

        if(t==0) break;

        i++;
    }
}

int main()
{
    int i=0;

    string k = "hello";
    int start = i;
    advance(i);
    cout<<k.substr(start, i-start )<<endl;
    return 0;
}