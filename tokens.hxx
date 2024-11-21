#include<iostream>
#include<exception>
#include<string>
#include<vector>
#include<map>
using namespace std;

enum tokType{
    //SINGLE CHAR TYPE
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA, DOT, MINUS, PLUS, SEMICLOLON, SLASH, STAR,

    // ONE OR TWO CHAR TOKEN
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, LESS, LESS_EQUAL,GREATER,GREATER_EQUAL,

    //LITERALS
    IDENTIFIER, STRING, NUMBER,

    //KEYWORDS
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    EoF
};



class token{
    public:
    tokType tok;
    int line_number;
    string lexeme;
    string literal;

    token(tokType type,string text, int line):
    tok(type),lexeme(text), line_number(line)
    {}

    token(tokType Type, string text, string text2, int line):
    tok(Type), lexeme(text), literal(text2), line_number(line) {}

};
