#pragma once
#include<iostream>
#include<variant>
#include<memory>
using namespace std;

class Expr;
class Binary;
class Grouping;
class Literal;
class Unary;
using Exprptr = std::shared_ptr<Expr>;
using Object = std::variant<int , bool,  std::monostate, double, string, std::shared_ptr<Binary>, std::shared_ptr<Grouping>, std::shared_ptr<Literal>, std::shared_ptr<Unary>>;

class visitor {
   public:
   virtual Object visitBinary ( const Binary& obj )=0;
   virtual Object visitGrouping ( const Grouping& obj )=0;
   virtual Object visitLiteral ( const Literal& obj )=0;
   virtual Object visitUnary ( const Unary& obj )=0;
};
class Expr { 
   public:
virtual Object accept(visitor& vis)const=0;
       virtual ~Expr() = default;
 };

class Binary : public Expr {
  public:
  Binary( std::shared_ptr<Expr> left, token Operator, std::shared_ptr<Expr> right) :
  left(std::move(left)), Operator(Operator), right(std::move(right)){} 
  Object accept( visitor& vis) const override { return vis.visitBinary(*this);}

  const Exprptr left;
  const token Operator;
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
  Literal( string value) :
  value(value){} 
  Object accept( visitor& vis) const override { return vis.visitLiteral(*this);}

  const string value;
};


class Unary : public Expr {
  public:
  Unary( token Operator , Exprptr right) :
  Operator(Operator), right(std::move(right)){} 
  Object accept( visitor& vis) const override { return vis.visitUnary(*this);}

  const token Operator ;
  const Exprptr right;
};


