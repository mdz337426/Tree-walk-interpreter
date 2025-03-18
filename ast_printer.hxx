
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
        string temp = std::holds_alternative<string>(obj) ? std::get<string>(obj) : to_string(std::get<double>(obj));
        s+=temp;
       }
       s+=")";
       return s;
    }

};