#include<iostream>
#include<fstream>
#include<vector>
#include<sys/wait.h>
#include<mutex>
#include<cstdlib>
#define ll long long


using namespace std;
std::mutex log_mutex;
void polling_booth(int boothid, int candidates, int votes)
{
    string boothfile = "boothid"+to_string(boothid)+".txt";
    std::ofstream os(boothfile);

    if(!os.is_open())
    {
        cout<<"error open file"<<endl;
        return;
    }

    srand(time(0)+boothid);
    std::ofstream logstream("log.txt", std::ios_base::app);
    

    for(int i=0; i<votes; i++)
    {
        int voter_id = boothid*10000+i;
        int voted = rand()%candidates +1;

        os<<"voter "<<voter_id<<" voted to candidate "<<voted<<endl;
        std::lock_guard<std::mutex> guard(log_mutex);
        logstream<<"Booth "<<boothid<<" voter "<<voter_id<<" voted for "<<voted<<endl;
    }

    logstream.close();
    os.close();

}

int main()
{
    vector<pid_t>v;
    int n;
    int m,booths;
    cin>>n>>m>>booths;

    int votes_per_pole= (n/booths);


    for(int i=0; i<booths; i++)
    {
        pid_t pid = fork();

        if(pid==0) 
        {
            polling_booth(i+1, m, votes_per_pole);
            exit(0);
        }
        else{
            v.push_back(pid);
        }
    }

    for(pid_t pid : v)
    {
        waitpid(pid, nullptr, 0 );
    }

    

    


    return 0;
}
