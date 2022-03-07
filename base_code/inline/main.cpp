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

class Derived: public Base
{
public:
    inline void who() override   // 不写 inline 时隐含内联
    {
        cout << "I am Derived\n";
    }
};

int main()
{
    // 此处的虚函数 who(), 是通过类 Base 的具体对象 b 来调用的，编译期间就能确定，所以可以内联，但最终是否内联取决于编译器
    Base b;
    b.who();

    // 此处的虚函数是通过指针调用的，呈现多态性，需要在运行期间确定，所以不能内联
    Base *ptr = new Derived();
    ptr->who();

    // 因为 Base 有虚析构函数，所以 delete 时，会优先优先调用派生类的的析构函数，然后再调用基类的虚析构函数
    delete ptr;
    ptr = nullptr;

    system("pause");
    return 0;
}