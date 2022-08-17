#include <iostream> 
#include <memory> 
using namespace std;

void foo(shared_ptr<int> i) 
{ 
    (*i)++; 
} 

int main() 
{ 
    // auto pointer = new int(10); // illegal, no direct assignment 
    // Constructed a shared_ptr 
    auto pointer = make_shared<int>(10); 

    foo(pointer); 

    cout << *pointer << endl; // 11 
    // The shared_ptr will be destructed before leaving the scope 


    auto pointer1 = make_shared<int>(10); 
    auto pointer2 = pointer1;    // 引用计数 +1 
    auto pointer3 = pointer1;    // 引用计数 +1 
    int *p = pointer1.get();     // 这样不会增加引用计数 

    cout << *p << endl;
    cout << "pointer1.use_count() = " << pointer1.use_count() << endl; // 3 
    cout << "pointer2.use_count() = " << pointer2.use_count() << endl; // 3 
    cout << "pointer3.use_count() = " << pointer3.use_count() << endl; // 3

    pointer2.reset(); 
    cout << "reset pointer2:" << endl; 
    cout << "pointer1.use_count() = " << pointer1.use_count() << endl; // 2 
    cout << "pointer2.use_count() = " << pointer2.use_count() << endl; // 0, pointer2 已 reset 
    cout << "pointer3.use_count() = " << pointer3.use_count() << endl; // 2 

    pointer3.reset(); 
    cout << "reset pointer3:" << endl; 
    cout << "pointer1.use_count() = " << pointer1.use_count() << endl; // 1 
    cout << "pointer2.use_count() = " << pointer2.use_count() << endl; // 0 
    cout << "pointer3.use_count() = " << pointer3.use_count() << endl; // 0, pointer3 已 reset

    return 0;
}

