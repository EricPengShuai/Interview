/* 虚继承 */

#include <iostream>
using namespace std;

class A
{
public:
    A(int data) : ma(data) { cout << "A()" << endl; }
    ~A() { cout << "~A()" << endl; }
protected:
    int ma;
};

class B : virtual public A
{
public:
    B(int data) : A(data), mb(data) { cout << "B()" << endl; }
    ~B() { cout << "~B()" << endl; }
protected:
    int mb;
};

class C : virtual public A
{
public:
    C(int data) : A(data), mc(data) { cout << "C()" << endl; }
    ~C() { cout << "~C()" << endl; }
protected:
    int mc;
};

class D: public B, public C
{
public:
    // B C 虚继承 A 之后，ma 必须交给 D 初始化，所以这里必须调用 A(data)
    D(int data) : A(data), B(data), C(data) { cout << "D()" << endl; }
    ~D() { cout << "~D()" << endl; }
protected:
    int md;
};

int main()
{
    D d(1);
    return 0;
}
