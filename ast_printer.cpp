
#include<iostream>
#include"Expr.hxx"
#include<vector>

class ast_printer: public visitor{
    public:


    Object print(Exprptr expr)
    {
        return expr->accept(*this);
    }


    Object visitBinary(const Binary& obj)
    {
        vector<Exprptr>v;
        v.push_back(obj.left);
        v.push_back(obj.right);
        return parenthesize(obj.Operator.lexeme,v);
    }

    Object visitGrouping(const Grouping& obj)
    {
        vector<Exprptr>v;
        v.push_back(obj.expression);
        return parenthesize("group", v);

    }

     Object visitLiteral(const Literal& obj)
    {
        if(obj.value.size()==0) return "nil";
        return obj.value;
    }

     Object visitUnary(const Unary& obj)
    {
        vector<Exprptr>v;
        v.push_back(obj.right);
        return parenthesize(obj.Operator.lexeme,v);
    }


    string parenthesize(string name, vector<Exprptr>list)
    {
        string s = "";
        s+= "(" + name ;
       
       for(int i=0; i<list.size(); i++)
       {
        s+=" ";
        Object obj=list[i]->accept(*this);
        string temp = std::get<string>(obj);
        s+=temp;
       }
       s+=")";
       return s;
    }

};


int main()
{
    Exprptr exp = std::make_shared<Binary>(
        std::make_shared<Unary>(token(tokType::MINUS, "-", 1), std::make_shared<Literal>("123")),
        token(tokType::STAR, "*", 1),
        std::make_shared<Grouping>(std::make_shared<Literal>("45.67"))
    );

    // Print AST
    ast_printer printer;
    Object result = printer.print(exp);
    std::cout << std::get<string>(result) << std::endl;

    return 0;
}
    
