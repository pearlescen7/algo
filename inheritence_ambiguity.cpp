#include <iostream>

using namespace std;

class A
{
  int a;
  
  public:
    A() : a(20){}
    A(int _a) : a(_a){}
    const A& operator=(const A& _aobj)
    {
        a = _aobj.a;    
    }
    void print(){cout << a << endl;}
};

class B : public A 
{
    int b;
    
    public:
        B() : b(0){}
        B(int _a, int _b) : A(_a), b(_b){}
        const B& operator=(const B& _bobj)
        {
            A::operator=(_bobj);
            b = _bobj.b;
        }
        void print()
        {
            A::print();
            cout << b << endl;
        }
};

int main()
{
    cout<<"Hello World" << endl;
    B* b = new B(30, 10);
    b->print();
    B b2;
    b2 = *b;
    b2.print();
    b2.A::print();
    return 0;
}
