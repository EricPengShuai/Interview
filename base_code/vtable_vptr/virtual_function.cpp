//
// Created by pengshuai on 2022/3/8.
//

/**
 * 基类指针指向继承类对象，则调用继承类对象的函数；
 * int main()必须声明为Base类的友元，否则编译失败。 编译器报错： ptr无法访问私有函数。
 * 当然，把基类声明为public， 继承类为private，该问题就不存在了。
 */

#include<iostream>
using namespace std;

/*class Derived;

class Base
{
private:
    virtual void fun() { cout << "Base Fun"; }
    friend int main();
};

class Derived: public Base
{
public:
    void fun() override { cout << "Derived Fun"; }
};

int main()
{
    Base *ptr = new Derived;
    ptr->fun();
    return 0;
}*/

class Derived;

class Base
{
public:
    virtual void fun() { cout << "Base Fun"; }
};

class Derived: public Base
{
private:
    void fun() override { cout << "Derived Fun"; }
};

int main()
{
    Base *ptr = new Derived;
    ptr->fun(); // Derived Fun
    return 0;
}