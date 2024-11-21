#include<iostream>
using namespace std;

template<typename R>
class Visitor;

template<typename R>
class shape
{
    public:
    virtual ~shape()=default;

    virtual Visitor<R> accept(Visitor<R>& visitor)=0;
};


template<typename T>
class circle: public shape<T>{
    public:
    int radius;
    Visitor<T> accept(Visitor<T>& v) override {
        return v.visit_circle(*this);
    }
};

template<typename T>
class rectangle: public shape<T>{
    public:
    int l;
    int b;
    Visitor<T> accept(Visitor<T>& v){
        return  v.visit_rectangle(*this);
    }    
};

template <typename T>
class Visitor{
    public:
    virtual T visit_circle(shape<circle<int>>& c)=0;
    virtual T visit_rectangle(shape<rectangle<int>>& r)=0;
};

template<class T>
class area : public Visitor{
    public:
    T visit_circle(T& c)override{
        return c.radius * c.radius * 3.14;
    }

    T visit_rectangle(T & r)override{
        return r.b * r.l ;
    }
};

int main()
{
    area<int> * a;
    circle<int> c;
    c.radius = 5;
    rectangle<int> r;
    r.l = 10;
    r.b = 10;
    cout<<(a->visit_circle(c))<<endl;
    cout<<(a->visit_rectangle(r))<<endl;

    
    
    return 0;
}
