//
// Created by pengshuai on 2022/3/8.
//

#include <iostream>
using namespace std;

class Base
{
public:
    virtual void fun (int x = 10) {
        cout << "Base::fun(), x = " << x << endl;
    }
};

class Derived: public Base
{
public:
    virtual void fun (int x = 20) {
        cout << "Derived::fun(), x = " << x << endl;
    }
};

int main()
{
    Derived d;
    Base *bp = &d;
    // 默认参数是静态绑定的，虚函数是动态绑定的。 默认参数的使用需要看指针或者引用本身的类型，而不是对象的类型。
    bp->fun();  // Derived::fun(), x = 10
    return 0;
}
