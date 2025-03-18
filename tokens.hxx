#pragma once
#include<iostream>
#include<exception>
#include<string>
#include<vector>
#include<variant>
#include<map>
using namespace std;

enum class TokenType{
    //SINGLE CHAR TYPE
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // ONE OR TWO CHAR TOKEN
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, LESS, LESS_EQUAL,GREATER,GREATER_EQUAL,

    //LITERALS
    IDENTIFIER, STRING, NUMBER,

    //KEYWORDS
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    EoF
};



class Token{
    public:
    TokenType type;
    int line;
    string lexeme;
    std::variant<double, string, nullptr_t> literal;

    Token(TokenType type, const string& lexeme, const variant<double, string, nullptr_t>& literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}

    string toString() const{
        string literalStr;
        if (std::holds_alternative<string>(literal)) {
            literalStr = std::get<string>(literal);
        } else if (std::holds_alternative<double>(literal)) {
            literalStr = to_string(std::get<double>(literal));
        } else {
            literalStr = "null";
        }
        return "Token(" + lexeme + ", " + literalStr + ")";
    }
};
