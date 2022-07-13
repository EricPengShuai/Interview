#include <iostream> 
#include <memory>
using namespace std;

struct Foo { 
    Foo() { cout << "Foo::Foo" << endl; } 
    ~Foo() { cout << "Foo::~Foo" << endl; } 
    void foo() { cout << "Foo::foo" << endl; } 
};

void f(const Foo &) { 
    cout << "f(const Foo&)" << endl; 
}

int main() {
    unique_ptr<Foo> p1(make_unique<Foo>());

    // p1 不空, 输出
    if (p1) p1->foo(); 

    unique_ptr<Foo> p2(move(p1));
    // p2 不空, 输出
    f(*p2); 

    // p2 不空, 输出
    if(p2) p2->foo(); 

    // p1 为空, 无输出
    if(p1) p1->foo(); 

    p1 = move(p2); 

    // p2 为空, 无输出
    if(p2) p2->foo(); 
    else cout << "p2 被销毁" << endl;

    // p1 不空, 输出
    if (p1) p1->foo(); 

    // Foo 的实例会在离开作用域时被销毁
    return 0;
}
