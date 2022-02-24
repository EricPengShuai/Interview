#include <iostream>
using namespace std;

class A
{
public:
    char b;
    virtual void func();
    virtual void func1() {};
    virtual void func2() {};
    static int c;
    static int d;
    static int f;
};

void A::func() {
    cout << "This is a function.\n";
}

int main() {
    cout << A::c << endl;
    cout << sizeof(A) << endl;  // 字节对齐: vptr=8（虚函数个数不影响), char=1
    return 0;
}
