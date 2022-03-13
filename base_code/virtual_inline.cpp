//
// Created by pengshuai on 2022/3/8.
//

/**
 * @brief 通常类成员函数都会被编译器考虑是否进行内联。
 * 但通过基类指针或者引用调用的虚函数必定不能被内联。
 * 当然，实体对象调用虚函数或者静态调用时可以被内联，虚析构函数的静态调用也一定会被内联展开。
 */

#include <iostream>
using namespace std;

/*
class Base
{
public:
    virtual void who()
    {
        cout << "I am Base\n";
    }
};
class Derived: public Base
{
public:
    void who() override
    {
        cout << "I am Derived\n";
    }
};

int main()
{
    // 注意这里虚函数 who() 通过实例化对象被调用（编译期间被解决）可以被内联
    Base b;
    b.who();    // I am Base

    // 这里虚函数通过指针被调用，不能内联
    Base *ptr = new Derived();
    ptr->who(); // I am Derived

    delete ptr;
    return 0;
}*/


#include <iostream>
using namespace std;
class Base
{
public:
    inline virtual void who()
    {
        cout << "I am Base\n";
    }
    virtual ~Base() = default;
};
class Derived : public Base
{
public:
    inline void who() override  // 不写inline时隐式内联
    {
        cout << "I am Derived\n";
    }
};

int main()
{
    // 此处的虚函数 who()，是通过类（Base）的具体对象（b）来调用的，编译期间就能确定了，所以它可以是内联的，但最终是否内联取决于编译器。
    Base b;
    b.who();    // I am Base

    // 此处的虚函数是通过指针调用的，呈现多态性，需要在运行时期间才能确定，所以不能为内联。
    Base *ptr = new Derived();
    ptr->who(); // I am Derived

    // 因为Base有虚析构函数（virtual ~Base() {}），所以 delete 时，会先调用派生类（Derived）析构函数，再调用基类（Base）析构函数，防止内存泄漏。
    delete ptr;

    return 0;
}