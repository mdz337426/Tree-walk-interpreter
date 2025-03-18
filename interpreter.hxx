#pragma once
#include "Expr.hxx"
#include<unordered_map>
#include "scanner.hxx"




class interpreter: public visitor{
    static Lox lox;

    Object evaluate(Exprptr expr)
    {
        return expr->accept(*this);
    }

    class RuntimeError : public std::runtime_error{
        Token token;
        public:
        explicit RuntimeError(Token token,  const std::string & message) : runtime_error(message), token(token){}
    };


    void checkNumberOperand(Token oprtr,Object& Operand)
    {
        if(std::holds_alternative<double>(Operand)) return;

        throw RuntimeError(oprtr, "operand must be a number");
    }

    void checkNumberOperands(Token oprtr,Object& left , Object& right)
    {
        if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
        throw RuntimeError(oprtr, "operand must be a number");

    }

    bool isTruthy(Object & obj){
        if(std::holds_alternative<std::nullptr_t>(obj)) return false;

        if(std::holds_alternative<bool>(obj)) return std::get<bool>(obj);

        return true;
    }

    bool isEqual(Object & left, Object & right)
    {
        if((std::holds_alternative<std::nullptr_t>(left)))
        {
            if(std::holds_alternative<std::nullptr_t>(right)) return true;
        }

        if(std::holds_alternative<std::nullptr_t>(left)) return false;
        return left==right;
    }

    public:

   
    Object visitLiteral(const Literal& obj)
    {
        if (std::holds_alternative<std::string>(obj.value)) {
            if (std::get<std::string>(obj.value).empty()) {
                return "nil";
            }
            return std::get<std::string>(obj.value);
        } 

        return obj.value;
    }

    Object visitGrouping(const Grouping&obj)
    {
        return evaluate(obj.expression);
    }


    Object visitUnary(const Unary&obj) {
        Object right = evaluate(obj.right);
        switch (obj.Operator.type) {
        case TokenType::MINUS: checkNumberOperand(obj.Operator, right);return -std::get<double>(right);
        case TokenType::BANG: return !(isTruthy(right)); 
        }
        // Unreachable.
        return "NIL";
    }

    Object visitBinary(const Binary&obj) {
        Object left = evaluate(obj.left);
        Object right = evaluate(obj.right);
        switch (obj.Operator.type) {
        case TokenType::MINUS:checkNumberOperands(obj.Operator, left, right); return std::get<double>(left) - std::get<double>(right);
        case TokenType::PLUS:  // cout<<std::get<double>(left)<<" "<<std::get<double>(right)<<endl;
                                 if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                                {
                                    return std::get<double>(left) + std::get<double>(right);
                                }
                             else if(std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
                                {
                                    return std::get<std::string>(left) + std::get<string>(right);
                                }
                            throw RuntimeError(obj.Operator, "operands must be two numbers or two strings");
        case TokenType::GREATER:checkNumberOperands(obj.Operator, left, right); return std::get<double>(left) > std::get<double>(right);
        case TokenType::GREATER_EQUAL:checkNumberOperands(obj.Operator, left, right); return std::get<double>(left) >= std::get<double>(right);
        case TokenType::LESS:checkNumberOperands(obj.Operator, left, right); return std::get<double>(left) < std::get<double>(right);
        case TokenType::LESS_EQUAL:checkNumberOperands(obj.Operator, left, right); return std::get<double>(left) <= std::get<double>(right);
        case TokenType::BANG_EQUAL:checkNumberOperands(obj.Operator, left, right); return !(isEqual(left, right));
        case TokenType::EQUAL_EQUAL: return isEqual(left, right);
        case TokenType::SLASH:checkNumberOperands(obj.Operator, left, right); return std::get<double>(left) / std::get<double>(right); 
        
        case TokenType::STAR:checkNumberOperands(obj.Operator, left, right); return std::get<double>(left) * std::get<double>(right);
        }
        // Unreachable.
        return "NIL";
    }

    

    std::string stringify(Object & value)
    {
        if(std::holds_alternative<std::nullptr_t>(value)) return "nil";

        if(std::holds_alternative<double>(value))
        {
            string text = to_string(std::get<double>(value));
            text = text.substr(0, text.size()-3);
            return text;
        }

        return std::get<string>(value);
    }


    void interpret(Exprptr expr)
    {
        try{
            Object value = evaluate(expr);
            cout<<stringify(value)<<"\n";
        }
        catch (RuntimeError err){
          //  lox.runtimeError(err);
          cout<<"runtime error at lox"<<endl;
        }

    }


  
    




    




};

Lox interpreter::lox;