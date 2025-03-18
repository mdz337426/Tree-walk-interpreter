#pragma once
#include"tokens.hxx"
#include<variant>
#include"Expr.hxx"
using Object = std::variant< Binary , Grouping , Literal , Unary >;

class visitor {
public:

virtual Object visitBinary( const Binary & obj )=0;

virtual Object visitGrouping( const Grouping & obj )=0;

virtual Object visitLitreral( const Literal & obj )=0;

virtual Object visitUnary( const Unary & obj )=0;


};
