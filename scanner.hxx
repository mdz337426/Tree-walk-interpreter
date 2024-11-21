

class Lox{
    bool Error;
    map<string , tokType>keywords;

    public:
    Lox()
    {
        Error=false;
        keywords.clear();
        keywords["and"]=tokType::AND;
        keywords["class"]=tokType::CLASS;
        keywords["else"]=tokType::ELSE;
        keywords["false"]=tokType::FALSE;
        keywords["for"]=tokType::FOR;
        keywords["fun"]=tokType::FUN;
        keywords["if"]=tokType::IF;
        keywords["nil"]=tokType::NIL;
        keywords["or"]=tokType::OR;
        keywords["print"]=tokType::PRINT;
        keywords["return"]=tokType::RETURN;
        keywords["super"]=tokType::SUPER;
        keywords["this"]=tokType::THIS;
        keywords["true"]=tokType::TRUE;
        keywords["var"]=tokType::VAR;
        keywords["while"]=tokType::WHILE;   
    }

    bool hadError()
    {
        return Error;
    }

    void ERROR(int line , string message)
    {
        report(line ,"", message);
    }

    void report(int line , string where, string message)
    {
    cout<<"[line "<<line<<"] Error"<<where<<": "<<message<<'\n';
    bool hadError = true;
    }

};

class Scanner{
    Lox lox;
    int curr_line;
    string source;
    vector<token> tokens;
    char advance(int& i)
    {
        return source[i++];
    }

    void addToken(token t)
    {
        tokens.push_back(t);
    }

    char peek(int i)
    {
        return source[i];
    }

    bool match(char expected,int& i)
    {
        if (i>= source.size()) return false;
        if(source[i]!=expected) return false;

        i++;
        return true;
    }

    void String(int& i)
    {
        int start = i;
        int temp_line = curr_line;
        while(peek(i)!='"' && source[i]!='\0')
        {
            if(peek(i)=='\n') curr_line++;
            advance(i);
        }

        if(source[i]=='\0')
        {
            lox.ERROR(curr_line, "Unterminated string.");
            return;
        }

        advance(i);
        string val = source.substr(start, i-start-1);
        addToken(token(tokType::STRING, val, "", temp_line));
    }

    bool isDigit(char c)
    {
        return c-'0'>=0 && c-'0'<=9;
    }

    char peek_next(int i)
    {
        if(i+i>= source.length()) return '\0';
        return source[i+1];
    }

    bool isAlpha(char c)
    {
        return (c>= 'a' && c<='z') || (c>='A' && c<='Z') || (c=='_');
    }

    bool isAlphanumeric(char c)
    {
        return isAlpha(c) || isdigit(c);
    }

    void number(int& i)
    {
        int start = i-1;
        while (isdigit(source[i])) advance(i);
        
        if(peek(i)=='.' && isDigit(peek_next(i))) advance(i);
        while (isdigit(source[i])) advance(i);
        addToken(token(tokType::NUMBER, source.substr(start, i-start),"", curr_line));
    }

    void identifier(int& i)
    {
        int start = i-1;
        while (isAlphanumeric(source[i])) advance(i);
        addToken(token(tokType::IDENTIFIER, source.substr(start, i-start), "", curr_line));
    }

    public:
    Scanner(string _line) : source(_line){ 
        curr_line=0;
    }

    
    void scanToken(int i)
    {
        while(source[i]!='\0')
        {
            char c = advance(i);
            switch (c)
            {
                case '(' : addToken(token(tokType::LEFT_PAREN, "(",    "", curr_line)) ;break;
                case ')' : addToken(token(tokType::RIGHT_PAREN, ")",   "", curr_line)) ;break;
                case '{' : addToken(token(tokType::LEFT_BRACE, "{",    "", curr_line)) ;break;
                case '}' : addToken(token(tokType::RIGHT_BRACE, "}",   "", curr_line)) ;break;
                case ',' : addToken(token(tokType::COMMA, ",", "", curr_line)) ;break;
                case '.' : addToken(token(tokType::DOT, ".", "", curr_line)) ;break;
                case '-' : addToken(token(tokType::MINUS, "-", "", curr_line)) ;break;
                case '+' : addToken(token(tokType::PLUS, "+", "", curr_line)) ;break;
                case ';' : addToken(token(tokType::SEMICLOLON, ";",    "", curr_line)) ;break;
                case '*' : addToken(token(tokType::STAR, "*", "", curr_line)) ;break;
                case '!': addToken( match('=',i) ? token(tokType::BANG_EQUAL, "!=", "", curr_line):token(tokType::BANG, "!", "", curr_line)); break;
                case '>': addToken(match('=',i) ? token(tokType::GREATER_EQUAL, ">=", "", curr_line) :token(tokType::GREATER, ">", "",  curr_line));break;
                case '<': addToken(match('=',i) ? token(tokType::LESS_EQUAL, "<=", "", curr_line):token(tokType::LESS, "<", "", curr_line));   break;
                case '=' : addToken(match('=',i) ? token(tokType::EQUAL_EQUAL, "==", "", curr_line) :token(tokType::EQUAL, "=", "", curr_line));break;
                case '/' : 
                        if(match('/',i))
                        {
                            while (peek(i)!='\n' && i<source.size()) advance(i);
                        }
                        else
                        {
                            addToken(token(tokType::SLASH, "/", "", curr_line));
                        }
                        break;
                case ' ':break;
                case '\r':break;
                case '\t':break;
                case '\n' : curr_line++ ; break;
                case '"' : String(i); break;

                default:
                        if(isdigit(c))
                            {
                                number(i);
                            }
                        else if (isAlpha(c))
                            {
                                identifier(i);
                            }
                        else{            
                                lox.ERROR(curr_line, "Unexpected chracter.");
                            }
                        break;
            }
        }

        if(source[i]=='\0') 
        {tokens.push_back(token(tokType::EoF, "", "", curr_line));
        }
    }

    vector<token> get_tokens(){
        return this->tokens;
    }
};