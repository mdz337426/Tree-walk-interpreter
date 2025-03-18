#pragma once
#include "scanner.hxx"
#include "tokens.hxx"
#include<memory>
#include "Expr.hxx"

class Parser{  
    private:
    static Lox lox;
    vector<Token> tokens;
    int current;  

    bool match(std::initializer_list<TokenType>types)
    {
        for(TokenType Type : types)
        {
            if(check(Type))
            {
                advance();
                return true;
            }
        }
        return false;
    }

    Token peek()
    {
        return tokens[current];
    }


    bool check(TokenType Type)
    {
        if(isAtEnd()) return false;
        return peek().type == Type;
    }

    Token advance()
    {
        if(!isAtEnd()) current++;
        return previous();
    }

    bool isAtEnd()
    {
        return current >= tokens.size();
    }

    Token previous()
    {
        return tokens[current-1];
    }
    
    std::shared_ptr<Expr> expression()
    {
        return equality();
    }
    
    std::shared_ptr<Expr> equality()
    {
        std::shared_ptr<Expr> expr = comparison();
        while(match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
        {
            Token Operator = previous();
            std::shared_ptr<Expr> right = comparison(); 
            expr = std::make_shared<Binary>(expr, Operator, right);
        }
        return expr;
    }

     std::shared_ptr<Expr> comparison()
    {
        std::shared_ptr<Expr> expr = term();

        while (match({TokenType::GREATER,TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
        {
            Token Operator = previous();
            std::shared_ptr<Expr> right = term();
            expr = std::make_shared<Binary>(expr, Operator, right);
        }
        return expr;  
    }


   

    std::shared_ptr<Expr> term()
    {
        std::shared_ptr<Expr> expr = factor();
        while(match({TokenType::MINUS, TokenType::PLUS}))
        {
            Token Operator = previous();
            std::shared_ptr<Expr> right = factor();
            expr = std::make_shared<Binary>(expr, Operator, right);
        }

        return expr;
    }

    std::shared_ptr<Expr> factor()
    {
        std::shared_ptr<Expr> expr = unary();
        while(match({TokenType::SLASH, TokenType::STAR}))
        {
            Token Operator = previous();
            std::shared_ptr<Expr> right = unary();
            expr = std::make_shared<Binary>(expr, Operator, right);
        }

        return expr;
    }

    std::shared_ptr<Expr> unary()
    {
        if(match({TokenType::BANG, TokenType::MINUS}))
        {
            Token Operator = previous();
            std::shared_ptr<Expr> right = unary();
            return make_shared<Unary>(Operator, right);
        }

        return primary();
    }

    std::shared_ptr<Expr> primary()
    {
        if(match({TokenType::FALSE})) return make_shared<Literal>("false");
        if(match({TokenType::TRUE})) return make_shared<Literal>("true");
        if(match({TokenType::NIL})) return make_shared<Literal>("null");

        if(match({TokenType::NUMBER, TokenType::STRING}))
        {
            return make_shared<Literal>(previous().literal);
        }

        if(match({TokenType::LEFT_PAREN}))
        {
            std::shared_ptr<Expr> expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
            return make_shared<Grouping>(expr);
        }

        throw error(peek(), "Expect expression.");
        
    }

    
    Token consume(TokenType Type, string Message )
    {
        if(check(Type)) return advance();

        throw runtime_error(peek().lexeme +  Message+ " at line number" + to_string(peek().line));
    }

    class ParseError : public runtime_error{
    public:
    explicit ParseError(const string & message) : runtime_error(message){}
    };
    
    ParseError error(Token t, std::string message)
    {
        ParseError p(message);
        lox.ERROR(t.line, message);
        throw p;
    }

    public:
    void synchronize()
    {
        advance();
        while (!isAtEnd())
        {
            if(previous().type == TokenType::SEMICOLON) return;
            switch (peek().type)
            {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            }

            advance();
        }     
    }
    
    static void Error(Token t, string message)
    {
        if(t.type== TokenType::EoF)
        {
            lox.report(t.line, " at end ", message);
        }
        else
        {
            lox.report(t.line, " at ' " + t.lexeme + "'", message);
        }
    }

    std::shared_ptr<Expr> parse(){
        try{
            return expression();
        }
        catch(ParseError error)
        {
            return nullptr;
        }
    }
    Parser(vector<Token>&tokens){
        this->tokens = tokens;
        this->current=0;
    }
};

Lox Parser::lox;