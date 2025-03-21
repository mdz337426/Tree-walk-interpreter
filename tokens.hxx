#pragma once
#include<string>
#include<vector>
#include<variant>
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
    std::string lexeme;
    std::variant<double, std::string, std::nullptr_t> literal;

    Token(TokenType type, const string& lexeme, const variant<double, std::string, std::nullptr_t>& literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}

    string toString() const{
        string literalStr;
        if (std::holds_alternative<std::string>(literal)) {
            literalStr = std::get<std::string>(literal);
        } else if (std::holds_alternative<double>(literal)) {
            literalStr = to_string(std::get<double>(literal));
        } else {
            literalStr = "null";
        }
        return "Token(" + lexeme + ", " + literalStr + ")";
    }
};
