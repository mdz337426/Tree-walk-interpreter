#include<iostream>
#include<vector>
#include<string>
#include"tokens.hxx"
using namespace std;





class Expr{};
class Binary : public Expr {
    public:
    Binary(Expr expr_l, token Operator, Expr expr_r){}
};
class Literals : public Expr{};
class Unary : public Expr{};
class Paren : public Expr{};

class parser{
    private :
    vector<token> tokens;
    int current;

    parser(vector<token> tok) : tokens(tok) , current(0){}

    Expr expression()
    {
        return equality();
    }

    Expr equality()
    {
        Expr expr = comparison();

        while (match(BANG_EQUAL, EQUAL_EQUAL))
        {
            token operator = previous();
            Expr right = comparison();
            expr = Binary(expr, operator, right); 
        }
        
    }
};

int main()
{
    return 0;
}


