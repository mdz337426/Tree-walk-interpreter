#pragma once
#include"tokens.hxx"
#include<variant>
#include"Expr.hxx"
using Object = std::variant< Binary , Grouping , Litreral , Unary >;

class visitor {
public:

virtual Object visitBinary  ( const Binary & obj )=0;

virtual Object visitGrouping  ( const Grouping & obj )=0;

virtual Object visitLitreral  ( const Litreral & obj )=0;

virtual Object visitUnary  ( const Unary & obj )=0;

};
