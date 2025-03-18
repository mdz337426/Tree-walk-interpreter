#pragma once
#include<iostream>
#include<variant>
#include<memory>
#include<string>
#include"tokens.hxx"
using namespace std;

class Expr;
class Binary;
class Grouping;
class Literal;
class Unary;
using Exprptr = std::shared_ptr<Expr>;
using Object = std::variant<bool,  std::monostate, double,std::nullptr_t, std::string, std::shared_ptr<Binary>, std::shared_ptr<Grouping>, std::shared_ptr<Literal>, std::shared_ptr<Unary>>;

class visitor {
   public:
   virtual Object visitBinary( const Binary& obj )=0;
   virtual Object visitGrouping( const Grouping& obj )=0;
   virtual Object visitLiteral( const Literal& obj )=0;
   virtual Object visitUnary( const Unary& obj )=0;
};
class Expr { 
   public:
      virtual Object accept(visitor& vis)const=0;
      virtual ~Expr() = default;

 };

class Binary : public Expr {
  public:
  Binary( std::shared_ptr<Expr> left, Token Operator, std::shared_ptr<Expr> right) :
  left(std::move(left)), Operator(Operator), right(std::move(right)){} 
  Object accept( visitor& vis) const override { return vis.visitBinary(*this);}

  const Exprptr left;
  const Token Operator;
  const Exprptr right;
};


class Grouping : public Expr {
  public:
  Grouping( Exprptr expression) :
  expression(std::move(expression)){} 
  Object accept( visitor& vis) const override { return vis.visitGrouping(*this);}

  const Exprptr expression;
};


class Literal : public Expr {
  
  public:
  Object value;
  Literal(const std::variant<double, std::string, std::nullptr_t>& value) {
    if (std::holds_alternative<std::string>(value)) {
        this->value = std::get<std::string>(value);
    } else if (std::holds_alternative<double>(value)) {
        this->value = std::get<double>(value);
    } else if (std::holds_alternative<std::nullptr_t>(value)) {
        this->value = "nullptr";
    }
}

Object accept( visitor& vis) const override { return vis.visitLiteral(*this);}

};


class Unary : public Expr {
  public:
  Unary( Token Operator , Exprptr right) :
  Operator(Operator), right(std::move(right)){} 
  Object accept( visitor& vis) const override { return vis.visitUnary(*this);}

  const Token Operator ;
  const Exprptr right;
};


