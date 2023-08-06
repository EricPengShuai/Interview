#include <iostream>
using namespace std;

// 先调用operator new 开辟内存空间、然后调用对象的构造函数（初始化）
void* operator new(size_t size)
{
    void *p = malloc(size);
    if (p == nullptr)
        throw bad_alloc();
    cout << "operator new addr:" << p << endl;
    return p;
}

// delete p; 调用 p 指向对象的析构函数、再调用 operator delete 释放内存空间
void operator delete(void *ptr) _NOEXCEPT
{
    cout << "operator delete addr:" << ptr << endl;
    free(ptr);
}

void* operator new[](size_t size)
{
    void *p = malloc(size);
    if (p == nullptr)
        throw bad_alloc();
    cout << "operator new[] addr:" << p << endl;
    return p;
}

void operator delete[](void *ptr) _NOEXCEPT
{
    cout << "operator delete[] addr:" << ptr << endl;
    free(ptr);
}

int main()
{
    try
    {
        int *p = new int;
        delete p;
    }
    catch(const bad_alloc& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    // new[] / delete[] 最好是配套使用，这里使用基础类型没问题
    // 如果涉及到类就不行了，因为 new [] 时往往会多分配 4 个字节存放多少个对象，以便在 delete [] 时候知道调用多少个对象的析构函数
    int *arr = new int[3]; 
    delete []arr;

    return 0;
}
