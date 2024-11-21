
class interpreter : public visitor{
    static Lox lox;

    Object evaluate(Exprptr expr)
    {
        return expr->accept(*this);
    }

    bool isEqual(Object left, Object right)
    {
        if(std::holds_alternative<std::monostate>(left) && std::holds_alternative<std::monostate>(right) ) return true;

        if(std::holds_alternative<std::monostate>(left)) return false;

        return left==right;
    }

     bool isTruthy(Object obj)
    {
        if(std::holds_alternative<std::monostate>(obj)) return false;

        if(std::holds_alternative<bool>(obj)) return std::get<bool>(obj);

        return true;
    }

     void checkNumberOperand(token Operator,Object operand)
    {
        if(std::holds_alternative<double>(operand)) return;

        throw RuntimeError(Operator, "Operand must be a number");
    }

    void checkNumberOperands(token Operator,Object left, Object right)
    {
        if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;

        throw RuntimeError(Operator, "operands must be numbers.");
    }

     class RuntimeError : public runtime_error{
        public:
        token Token;
        string Message;
        RuntimeError(token t, string message):Token(t), std::runtime_error(message), Message(message){}
    };

    string stringify(Object obj)
    {
        if(std::holds_alternative<std::monostate>(obj)) return "nil";

        if(std::holds_alternative<double>(obj))
        {
            string text = to_string(std::get<double>(obj));
            return text;
        }

        return std::get<string>(obj);
    }


    public:

    static void runtimeError(RuntimeError error)
    {
        cout<<error.Message<<" at line number "<<error.Token.line_number;
    }

    Object visitLiteral(const Literal& expr)
    {
        return expr.value;
    }

    Object visitGrouping(const Grouping& expr)
    {
        return evaluate(expr.expression);
    }

    Object visitUnary(const Unary& expr)
    {
        Object right = evaluate(expr.right);

        switch (expr.Operator.tok)
        {
        case tokType::BANG : return !(isTruthy(right));
        case tokType::MINUS:checkNumberOperand(expr.Operator, right);
                            return - (std::get<double>(right));
        }

        return std::monostate{};
    }

    
    Object visitBinary(Binary expr)
    {
        Object left = evaluate(expr.left);
        Object right = evaluate(expr.right);

        switch (expr.Operator.tok)
        {
                case tokType::MINUS:checkNumberOperands(expr.Operator, left, right);
                                 return std::get<double>(left) - std::get<double>(right);
                case tokType::SLASH:checkNumberOperands(expr.Operator, left, right);
                             return std::get<double>(left) / std::get<double>(right);
                case tokType::STAR :checkNumberOperands(expr.Operator, left, right);
                             return std::get<double>(left) * std::get<double>(right);
                case tokType::PLUS :
                            if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return std::get<double>(left) + std::get<double>(right);

                            if(std::holds_alternative<string>(left) && std::holds_alternative<string>(right)) return std::get<string>(left) + std::get<string>(right);

                            throw RuntimeError(expr.Operator, "operands must be two numebers or two strings.");

                            break;
                case tokType::GREATER : checkNumberOperands(expr.Operator, left, right);
                                 return std::get<double>(left) > std::get<double>(right);
                case tokType::GREATER_EQUAL :checkNumberOperands(expr.Operator, left, right);
                                     return std::get<double>(left) >= std::get<double>(right);
                case tokType::LESS : checkNumberOperands(expr.Operator, left, right);
                            return std::get<double>(left) < std::get<double>(right);
                case tokType::LESS_EQUAL:checkNumberOperands(expr.Operator, left, right);
                                 return std::get<double>(left) <= std::get<double>(right);
                case tokType::BANG_EQUAL : return !isEqual(left, right);
                case tokType::EQUAL_EQUAL: return  isEqual(left, right);
        }

        return std::monostate{};
    }

    void interpret(std::shared_ptr<Expr> expr)
    {
        try
        {
            Object value = evaluate(expr);
            cout<<stringify(value)<<endl;
        }
        catch(RuntimeError error)
        {
            runtimeError(error);  
        }
        
    }


};