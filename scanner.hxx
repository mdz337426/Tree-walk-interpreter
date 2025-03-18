#pragma once
#include<unordered_map>
#include<optional>
#include"tokens.hxx"
#include"variant"


class RuntimeError {
    public:
        RuntimeError(const std::string& message, const Token& token)
            : message(message), token(token) {}
    
        std::string getMessage() const {
            return message;
        }
    
        Token getToken() const {
            return token;
        }
    
    private:
        std::string message;
        Token token;
    };

class Lox{
    bool Error;
    public:
    static unordered_map<string , TokenType>keywords;
    Lox(){
        this->Error = false;
    }
    static void initializeKeywords() {
        keywords = {
            {"and", TokenType::AND},
            {"class", TokenType::CLASS},
            {"else", TokenType::ELSE},
            {"false", TokenType::FALSE},
            {"for", TokenType::FOR},
            {"fun", TokenType::FUN},
            {"if", TokenType::IF},
            {"nil", TokenType::NIL},
            {"or", TokenType::OR},
            {"print", TokenType::PRINT},
            {"return", TokenType::RETURN},
            {"super", TokenType::SUPER},
            {"this", TokenType::THIS},
            {"true", TokenType::TRUE},
            {"var", TokenType::VAR},
            {"while", TokenType::WHILE}
        };
    }

    static bool hadRunTimeError;
    static bool hadError;

    bool haderror()
    {
        return Error;
    }

    void ERROR(int line , string message)
    {
        report(line ,"", message);
    }

    void report(int line , string where, string message)
    {
    cout<<"[line "<<line<<"] Error "<<where<<" : "<<message<<'\n';
    bool hadError = true;
    }

    static void runtimeError(RuntimeError error)
    {
        std::cerr<<(error.getMessage()+ "\n [line"+to_string(error.getToken().line)+"]");
        hadRunTimeError = true;
    }

};

bool Lox::hadError = false;

bool Lox::hadRunTimeError = false;

std::unordered_map< std::string, TokenType> Lox :: keywords;

class Scanner{
    string source;
    vector<Token> tokens;
    int start;
    int current;
    int line;

    char advance()
    {
        current++;
        return source[current-1];
    }

    void addToken(TokenType type)
    {
        addToken(type, nullptr);
    }

    void addToken(TokenType type, std::variant<double, string, nullptr_t> literal)
    {
        string text = source.substr(start, current-start);
        tokens.push_back(Token(type, text, literal, line));
    }

    char peek()
    {
        if(isAtEnd()) return '\0';
        return source[current];
    }

    bool match(char expected)
    {
        if (isAtEnd()) return false;
        if(source[current]!=expected) return false;
        current++;
        return true;
    }

    void Estring()
    {
        while(peek()!='"' && !(isAtEnd()))
        {
            if(peek()=='\n') line++;
            advance();
        }

        if(source[current]=='\0')
        {
            lox.ERROR(line, "Unterminated string.");
            return;
        }

        advance();
        string val = source.substr(start, current-start+1);
        addToken(TokenType::STRING, val);
    }

    bool isDigit(char c)
    {
        return c-'0'>=0 && c-'0'<=9;
    }

    char peek_next()
    {
        if(current+1 >= source.size()) return '\0';
        return source[current+1];
    }

    bool isAlpha(char c)
    {
        return (c>= 'a' && c<='z') || (c>='A' && c<='Z') || (c=='_');
    }

    bool isAlphanumeric(char c)
    {
        return isAlpha(c) || isdigit(c);
    }

    void number()
    {
        while (isdigit(source[current])) advance();
        if(peek()=='.' && isDigit(peek_next())) advance();
        while (isdigit(source[current])) advance();
        addToken(TokenType::NUMBER, std::stod(source.substr(start, current-start)));
    }

    void identifier()
    {
        while (isAlphanumeric(peek())) advance();
        string text = source.substr(start, current-start);
        TokenType type;
        if(Lox::keywords.find(text)!= Lox::keywords.end())
        {
            type = Lox::keywords[text];
              
        }
        else
        {
            type = TokenType::IDENTIFIER;
        }
        addToken(type);
    }

    bool isAtEnd()
    {
        return current >= source.size();
    }

    public:
    static Lox lox;
    Scanner(string source) : source(source){ 
        this->line=1;
        this->current=0;
        this->start=0;
    }
    
    vector<Token> scanTokens()
    {
        while(!isAtEnd())
        {
            start = current;
            scanToken();
        }

        tokens.push_back(Token(TokenType::EoF, "", nullptr, line));
        return tokens;
    }

    void scanToken()
    {
        char c = advance();
        switch (c) {
            case '(': addToken(TokenType::LEFT_PAREN); break;
            case ')': addToken(TokenType::RIGHT_PAREN); break;
            case '{': addToken(TokenType::LEFT_BRACE); break;
            case '}': addToken(TokenType::RIGHT_BRACE); break;
            case ',': addToken(TokenType::COMMA); break;
            case '.': addToken(TokenType::DOT); break;
            case '-': addToken(TokenType::MINUS); break;
            case '+': addToken(TokenType::PLUS); break;
            case ';': addToken(TokenType::SEMICOLON); break;
            case '*': addToken(TokenType::STAR); break;
            case '!': addToken(match('=') ?TokenType::BANG_EQUAL : TokenType::BANG) ; break;
            case '=': addToken(match('=') ?TokenType::EQUAL_EQUAL : TokenType::EQUAL) ; break;
            case '<': addToken(match('=') ?TokenType::LESS_EQUAL : TokenType::LESS) ; break;
            case '>': addToken(match('=') ?TokenType::GREATER_EQUAL : TokenType::GREATER) ; break;
            case '/':  if(match('/')){
                            while(peek() != '\n' && !(isAtEnd())) advance();
                            }
                        else{
                            addToken(TokenType::SLASH);
                        }
                        break;
            case '\t': break;
            case '\n':line++; break;
            case '"': Estring(); break;
            case ' ':break;
            case 'o': if(peek()=='r') {
                        addToken(TokenType::OR);
                        }
                        break;
            default:
                        if(isDigit(c))
                        {
                            number();
                        }
                        else if(isAlpha(c))
                        {
                            identifier();
                        }
                        else{
                            lox.ERROR(line, "Unexpected character."+c);
                        }
            }
    }
};

Lox Scanner::lox;