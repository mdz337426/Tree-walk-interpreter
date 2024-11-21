class Parser{  
    private:
    static Lox lox;
    vector<token> tokens;
    int current;
    std::shared_ptr<Expr> expression()
    {
        return equality();
    }

     std::shared_ptr<Expr> comparison()
    {
        std::shared_ptr<Expr> expr = term();

        while (match({tokType::GREATER,tokType::GREATER_EQUAL, tokType::LESS, tokType::LESS_EQUAL}))
        {
            token Operator = previous();
            std::shared_ptr<Expr> right = term();
            expr = std::make_shared<Binary>(expr, Operator, right);
        }
        return expr;  
    }

    std::shared_ptr<Expr> equality()
    {
        std::shared_ptr<Expr> expr = comparison();
        while(match({tokType::BANG_EQUAL, tokType::EQUAL_EQUAL}))
        {
            token Operator = previous();
            std::shared_ptr<Expr> right = comparison(); 
            expr = std::make_shared<Binary>(expr, Operator, right);
        }
        return expr;
    }

    bool match(std::initializer_list<tokType>types)
    {
        for(tokType Type : types)
        {
            if(check(Type))
            {
                advance();
                return true;
            }
        }
        return false;
    }

    bool check(tokType Type)
    {
        if(isAtEnd()) return false;
        return peek().tok == Type;
    }

    token advance()
    {
        if(!isAtEnd()) current++;
        return previous();
    }

    bool isAtEnd()
    {
        return current == tokens.size();
    }

    token previous()
    {
        return tokens[current-1];
    }

   

    std::shared_ptr<Expr> term()
    {
        std::shared_ptr<Expr> expr = factor();
        while(match({tokType::MINUS, tokType::PLUS}))
        {
            token Operator = previous();
            std::shared_ptr<Expr> right = factor();
            expr = std::make_shared<Binary>(expr, Operator, right);
        }

        return expr;
    }

    std::shared_ptr<Expr> factor()
    {
        std::shared_ptr<Expr> expr = unary();
        while(match({tokType::SLASH, tokType::STAR}))
        {
            token Operator = previous();
            std::shared_ptr<Expr> right = unary();
            expr = std::make_shared<Binary>(expr, Operator, right);
        }

        return expr;
    }

    std::shared_ptr<Expr> unary()
    {
        if(match({tokType::BANG, tokType::MINUS}))
        {
            token Operator = previous();
            std::shared_ptr<Expr> right = unary();
            return make_shared<Unary>(Operator, right);
        }

        return primary();
    }

    std::shared_ptr<Expr> primary()
    {
        if(match({tokType::FALSE})) return make_shared<Literal>("false");
        if(match({tokType::TRUE})) return make_shared<Literal>("true");
        if(match({tokType::NIL})) return make_shared<Literal>("null");

        if(match({tokType::NUMBER, tokType::STRING}))
        {
            return make_shared<Literal>(previous().literal);
        }

        if(match({tokType::LEFT_PAREN}))
        {
            std::shared_ptr<Expr> expr = expression();
            consume(tokType::RIGHT_PAREN, "Expect ')' after expression.");
            return make_shared<Grouping>(expr);
        }
        
    }

    token peek()
    {
        return tokens[current];
    }

    token consume(tokType Type, string Message )
    {
        if(check(Type)) return advance();

        throw runtime_error(peek().lexeme +  Message+ " at line number" + to_string(peek().line_number));
    }

    class ParseError : public runtime_error{
    public:
    explicit ParseError(const string & message) : runtime_error(message){}
    };
    
    ParseError error(token t, string message)
    {
        ParseError p(message);
        lox.ERROR(t.line_number, message);
        return p;
    }

    public:
     void synchronize()
    {
        advance();
        while (!isAtEnd())
        {
            if(previous().tok == tokType::SEMICLOLON) return;
            switch (peek().tok)
            {
            case tokType::CLASS:
            case tokType::FUN:
            case tokType::VAR:
            case tokType::FOR:
            case tokType::IF:
            case tokType::WHILE:
            case tokType::PRINT:
            case tokType::RETURN:
                return;
            }

            advance();
        }
        
    }
    
    static void Error(token t, string message)
    {
        if(t.tok== tokType::EoF)
        {
            lox.report(t.line_number, " at end ", message);
        }
        else
        {
            lox.report(t.line_number, " at ' " + t.lexeme + "'", message);
        }
    }

    Parser(vector<token>&Tokens){
        this->tokens = Tokens;
        this->current=0;
    }
};
