//
// Created by pengshuai on 2022/3/31.
//

#include <iostream>

using namespace std;

class A
{
public:
    explicit A(int _a):a(_a){};
    friend int geta(A &ca); // 友元函数
    friend class B;
private:
    int a;
};

class B
{
public:
    static int getb(A ca) {
        return ca.a;
    }
};

int geta(A &ca)
{
    return ca.a;
}

int main()
{
    A a(3);
    cout << geta(a) << endl;

    __attribute__((unused)) B b;
    cout << B::getb(a) << endl;
    return 0;
}