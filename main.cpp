#include<vector>
#include<string>
#include<variant>
#include<memory>
#include"tokens.hxx"
#include"scanner.hxx"
#include"Expr.hxx"
#include"parser.hxx"
#include"evaluator.hxx"



using namespace std;

void run(string s)
{
    Scanner sc(s);
    cout<<"scanning..."<<endl;
    sc.scanToken(0);
    cout<<"scanned successfully"<<endl;
    vector<token> vt = sc.get_tokens();
    cout<<"parsing..."<<endl;
    Parser p(vt);
    p.synchronize();
    cout<<"parsed successfully"<<endl;
    

}
int main()
{
    string s;
    for(;;)
    {
        cout<<"> ";
        cin>>s;
        if(s=="\n") break;
        run(s);
    }

    return 0;
}
