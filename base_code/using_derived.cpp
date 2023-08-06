//
// Created by pengshuai on 2022/3/31.
//

#include <iostream>
using namespace std;

class Base{
public:
    void f(){ cout<<"f()"<<endl;
    }
    void f(int n){
        cout<<"Base::f(int)"<<endl;
    }
};

class Derived : private Base {
public:
    using Base::f;
    // void f() { cout << "Derived::f()" << endl; }

    void f(int n) {
        cout<<"Derived::f(int)"<<endl;
    }
};

int main()
{
    // Base b;
    Derived d;
    d.f();
    d.f(1);
    return 0;
}