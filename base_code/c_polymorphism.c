//
// Created by pengshuai on 2022/3/26.
//

#include <stdio.h>

typedef void (*pf) ();  // 重定义一个函数指针的类型

typedef struct A
{
    pf _f;
} A;

typedef struct B
{
    A _b;   // 在子类中定义一个基类的对象就可以实现父类的继承
} B;

void FunA()
{
    printf("%s\n", "Base A::fun()");
}

void FunB()
{
    printf("%s\n", "Derived B::fun()");
}

int main()
{
    A a;
    B b;

    a._f = FunA;
    b._b._f = FunB;

    A *pa = &a;
    pa->_f();
    pa = (A *) &b;   // 父类指针指向指向子类的对象，由于类型不匹配所以需要进行强制类型转换
    pa->_f();
    return 0;
}