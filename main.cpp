#include<vector>
#include<string>
#include<variant>
#include<fstream>
#include<sstream>
#include<iostream>
#include "tokens.hxx"
#include"scanner.hxx"
#include "parser.hxx"
#include "ast_printer.hxx"
#include "interpreter.hxx"

void run(std::string& s)
{
    Scanner scanner(s);
    vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens);
    std::shared_ptr<Expr> expression = parser.parse();
    if(scanner.lox.hadError) return;
    ast_printer as;
    Object result = as.print(expression);
    if(std::holds_alternative<string>(result))
    {
        std::cout<<std::get<string>(result)<<"\n";
    }
    interpreter inp;
    inp.interpret(expression);
}




void runFile(char* s){
    std::ifstream inputfile(s);
    if(!inputfile.is_open())
    {
        std::cerr<<"Eror: file '"<<s<<"' could not be opened!"<<"\n";
    }

    std::stringstream buffer;
    buffer<< inputfile.rdbuf();
    std::string fileContent = buffer.str();
    inputfile.close();
    run(fileContent);
}

void runPrompt(){
    std::string line;
    while (true) {
        std::cout << "> "; 
        std::getline(std::cin, line);
        if (line.empty()) break; 
        run(line); 
    }

}



int main(int argc, char* argv[])
{
    if(argc<2)
    {
        std::cout<<"Usage: Cpplox [script]"; 
    }
    else if(argc==2)
    {
        runFile(argv[1]);
    }
    else
    {
        runPrompt();
    }
    return 0;
}
