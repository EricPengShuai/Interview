#include <iostream>
using namespace std;

class Base
{
protected:
    int x;
public:
    virtual void fun() = 0; // 纯虚函数
    explicit Base(int i) { x = i; }
    virtual ~Base() {
        cout << "Destructor: Base." << endl;
    }
};

class Derived: public Base
{
    int y;  // 默认是 private
public:
    Derived(int i, int j) : Base(i) {
        y = j;
        cout << "Constructor: Derived." << endl;
    }
    void fun() override {
        cout << "x = " << x << ", y = " << y << endl;
    }
    ~Derived() override {
        cout << "Destructor: Derived." << endl;
    }

};

int main() {
    //    Base base(0);
    //    Base *b = new Base(); // 不能创建抽象类的对象
    Derived derived(1, 2);
    derived.fun();
    return 0;
}
