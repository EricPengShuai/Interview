#include <iostream>
using namespace std;

class A
{
public:
    virtual void f() {
        cout << "Base A::f() " << endl;
    }
};

class B: public A
{
public:
    void f() override {
        cout << "Derived B::f() " << endl;
    }
};

int main() {
    A a;
    B b;

    A* pa = &a;
    pa->f();    // 父类指针指向父类对象，调用父类的函数

    pa = &b;
    pa->f();    // 父类指针指向子类对象，实现多态，调用派生类同名函数
    return 0;
}
