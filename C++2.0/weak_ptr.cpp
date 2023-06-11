#include <iostream> 
#include <memory>
using namespace std;

struct A; struct B;

struct A { 
    // shared_ptr<B> pointer;  // 循环计数
    weak_ptr<B> pointer; 
    ~A() { 
        cout << "A 被销毁" << endl; 
    } 
}; 

struct B { 
    // shared_ptr<A> pointer;  // 循环计数
    weak_ptr<A> pointer; 
    ~B() { 
        cout << "B 被销毁" << endl; 
    } 
}; 

int main() {
    auto a = make_shared<A>();
    auto b = make_shared<B>();

    cout << "a.count = " << a.use_count() << endl;  // 1
    cout << "b.count = " << b.use_count() << endl;  // 1
    
    a->pointer = b;
    b->pointer = a;

    cout << "a.count = " << a.use_count() << endl;  // 1
    cout << "b.count = " << b.use_count() << endl;  // 1
    return 0;
}
