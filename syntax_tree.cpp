#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
using namespace std;

class Expr{};
class Binary : public Expr {};
class Literals : public Expr{};
class Unary : public Expr{};
class Paren : public Expr{};


void defineType(ostream& writer,string &basename, string &className, string& fields)
{
    
    writer<<"class "+className+" : public "+basename+" {\n";
    //constructor
    writer<<"  public:"<<endl; 
    istringstream ss(fields);
    vector<string> fieldsList;
    string fild;
    while (getline(ss, fild, ','))
    {
        fieldsList.push_back(fild);
    }

    writer<<"  "+className+"("+fields+") :"<<endl;
    string temp="";
    writer<<"  ";

   
    for(string fld : fieldsList)
    {
        istringstream field(fld);

        string name, type;
        field>>type;
        field>>name;

        if(name=="left" || name=="right" || name=="expression") writer<<name<<"(std::move("+name+")), ";
        else writer<<name<<"("<<name<<"), ";
    }
    //remove last ", "
    writer.seekp(-2, ios_base::end);


    writer<<"{} "<<endl;

    writer<<"  Object accept( visitor& vis) const override";
    writer<<" { return vis.visit"<<className<<"(*this);}"<<endl;

    writer<<endl;

    for(string fld : fieldsList)
    {
        writer<<"  const"+fld+";"<<endl;
    }
    writer<<"};"<<endl<<endl<<endl;
}



void defineVisitor(ostream&writer,  vector<string>list)
{
   
    writer<<"using Object = std::variant< string, ";
    for(string type : list)
    {
        istringstream iss(type);
        string classname;
        getline(iss, classname , ' ');
        writer<<"std::shared_ptr<"<<classname<<">, ";
    }
    writer.seekp(-2, ios_base::end);
    writer<<">;"<<endl;
    writer<<""<<endl;
    writer<<"class visitor {"<<endl;
    writer<<"   public:"<<endl;
    
    for(string item : list)
    {
        istringstream iss(item);
        string classname;
        getline(iss, classname,' ');
        writer<<"   virtual Object visit"+classname<<" ( const " + classname+"& obj )=0;"<<endl;
    }

    writer<<"};"<<endl;
}



void defineAst(string outputDir,string  basename,vector<string>list){
    string path =basename+".hxx";
    std::ofstream writer(path);
    if(!writer.is_open())
    {
        std::cerr<<"Error opening file"<<endl;
        exit(1);
    } 
    writer<<"#pragma once"<<endl;
    writer<<"#include<iostream>"<<endl;
    writer<<"#include\"tokens.hxx\""<<endl;
    writer<<"#include<variant>"<<endl;
    writer<<"#include<memory>"<<endl;
    writer<<"using namespace std;"<<endl<<endl;

    writer<<"class Expr;"<<endl;
    for(string type: list)
    {
        istringstream iss(type);
        string className, fields;
        getline(iss, className, ' ');
        writer<<"class "<<className<<";"<<endl;
    }
    writer<<"using Exprptr = std::unique_ptr<Expr>;"<<endl;

    defineVisitor(writer, list);

    writer<<"class "+basename+" { "<<endl;
    writer<<"   public:"<<endl;
    writer<<"virtual Object accept(visitor& vis)const=0;"<<endl;
    writer<<"       virtual ~"+basename+"() = default;"<<endl;
    writer<<" };"<<endl<<endl;

    

    for(string type : list)
    {
        istringstream iss(type);
        string className, fields;
        getline(iss, className, ':');
        getline(iss,fields, ':');
        className.pop_back();
        defineType(writer, basename, className, fields);
    }
    writer.close();
}

int main(int argc, char* argv[])
{
    string outputDir = "Expr";
    vector<string>list = {
        "Binary : Exprptr left, token Operator, Exprptr right",
        "Grouping : Exprptr expression",
        "Literal : string value",
        "Unary : token Operator , Exprptr right"
    };
    defineAst(outputDir, "Expr",list); 
    return 0;
}