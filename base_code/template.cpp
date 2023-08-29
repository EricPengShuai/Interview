//
// Created by pengshuai on 2022/4/19.
//

#include <iostream>
#include <cstring>
using namespace std;


/*************** 函数模板 ***************/
template<typename T>
bool compare(T a, T b)
{
    cout << "template compare" << endl;
    return a > b;
}

// 模板特例化
template<>
bool compare<const char *>(const char * a, const char * b)
{
    cout << "compare<const char *>\n";
    return strcmp(a, b);
}

// 函数
bool compare(const char * a, const char * b)
{
    cout << "compare\n";
    return strcmp(a, b);
}

/*************** 顺序栈类模板 ****************/
template<typename T>
class SeqStack
{
public:
    SeqStack(int size = 10) 
     : _pstack(new T[size])
    , _top(0)
    , _size(size)
    {}

    ~SeqStack() {
        delete []_pstack;
        _pstack = nullptr;
    }

    SeqStack(const SeqStack<T> &stack)
        : _top(stack._top)
        , _size(stack._size) // 提供拷贝构造函数，避免浅拷贝
    {
        _pstack = new T[_size];
        for (int i = 0; i < _top; ++ i) {
            _pstack[i] = stack._pstack[i];
        }
    }

private:
    T *_pstack;
    int _top;
    int _size;
};

/**
 * @brief 容器的空间配置器 allocator
 * 1. 内存开辟/内存释放
 * 2. 对象构造/对象析构 
 * */
template<typename T>
struct Allocator
{
    T* allocate(size_t size) // 负责内存开辟
    {
        return (T*)malloc(sizeof(T) * size);
    }

    void deallocate(void *p) // 负责内存释放
    {
        free(p);
    }

    void construct(T *p, const T &val) // 负责对象构造
    {
        new (p) T(val); // 定位 new: 在地址 p 创建值为 val 的对象
    }
    
    template<typename... Types> // 接受不定参的构造方法
    void construct(T* ptr, Types&&... args)
    {
        new (ptr) T(std::forward<Types>(args)...);
    }

    void destroy(T *p) // 负责对象析构
    {
        p->~T();
    }
};

/*************** vector 类模板 ****************/
template<typename T, typename Alloc = Allocator<T>>
class Vector
{
public:
    Vector(int size = 10)
    {
        // 需要把内存开辟和对象构造分开处理, 不能是直接 _first = new T[size]
        // 因为 new 既分配空间又会调用相应的构造函数, 实际上这里只需要分配空间
        _first = _allocator.allocate(size);
        _last = _first;
        _end = _first + size;
    }

    ~Vector()
    {
        // 析构容器的有效元素，然后释放 _first 指针指向的堆内存
        for (T *p = _first; p != _last; ++ p) 
        {
            _allocator.destroy(p); // 把 _first 指针指向的数组的有效元素进行析构
        }
        _allocator.deallocate(_first); // 释放堆上的数组内存
        _first = _last = _end = nullptr;
    }

    Vector(const Vector<T> &rhs)
    {
        int size = rhs._end - rhs._first;
        _first = _allocator.allocate(size); // 分配 size 大小内存
        int len = rhs._last - rhs._first;
        for (int i = 0; i < len; ++ i) { // 构造 len 个有效的T对象
            _allocator.construct(_first + i, rhs.first[i]);
        }
        _last = _first + len;
        _end = _first + size;
    }

    Vector<T>& operator=(const Vector<T> &rhs)
    {
        if (this == &rhs) return *this;

        for (T *p = _first; p != _last; ++ p) // 析构有效对象 
        {
            _allocator.destroy(p);
        }
        _allocator.deallocate(_first); // 释放所有内存

        int size = rhs._end - rhs._first;
        _first = _allocator.allocate(size); // 分配 size 大小内存

        int len = rhs._last - rhs._first;
        for (int i = 0; i < len; ++ i) // 构造 len 个有效的T对象 
        {
            _allocator.construct(_first + i, rhs.first[i]);
        }
        _last = _first + len;
        _end = _first + size;
    }

    // void push_back(const T& val) // 向容器末尾添加元素
    // {
    //     if (full()) {
    //         expand();
    //     }
    //     _allocator.construct(_last, val);
    //     ++ _last;
    // }

    // void push_back(T&& val) // 接受右值
    // {
    //     if (full()) {
    //         expand();
    //     }
    //     _allocator.construct(_last, move(val));
    //     ++ _last;
    // }

    // 通过万能引用实现 push_back 的两个版本
    template<typename Types>
    void push_back(Types&& args)
    {
        if (full()) {
            expand();
        }
        // 1. 引用折叠 &+&& => &, &&+&& => &&
        // 2. 类型完美转发 forward
        _allocator.construct(_last, std::forward<Types>(args));
        ++ _last;
    }

    template<typename... Types>
    void emplace_back(Types&&... args)
    {
        if (full()) {
            expand();
        }
        // 1. 引用折叠 &+&& => &, &&+&& => &&
        // 2. 类型完美转发 forward
        _allocator.construct(_last, std::forward<Types>(args)...);
        ++ _last;
    }
    
    void pop_back() // 从容器末尾删除元素
    {
        if (empty()) {
            return;
        }
        -- _last;
        _allocator.destroy(_last);
    }

    T back() const  { return *(_last - 1); }

    bool full() const { return _last == _end; }
    bool empty() const { return _first == _last; }
    int size() const { return _last - _first; }

private:
    T *_first;
    T *_last;
    T *_end;
    Alloc _allocator;

    void expand()
    {
        int size = _end - _first;
        T *ptmp = _allocator.allocate(2 * size); // 分配 2*size 大小内存
        int len = _last - _first;
        for (int i = 0; i < size; ++ i) { // 构造 size 个有效的T对象
            _allocator.construct(ptmp + i, _first[i]);
        }

        // 先析构再free
        for (T *p = _first; p != _last; ++ p) {
            _allocator.destroy(p);
        }
        _allocator.deallocate(_first);
        _first = ptmp;
        _last = _first + size;
        _end = _first + 2 * size;
    }
};

/* 测试类 */
class Test
{
public:
    Test() { cout << "Test()\n"; }
    Test(int a) { cout << "Test(int)\n"; }
    Test(int a, int b) { cout << "Test(int, int)\n"; }
    ~Test() { cout << "~Test()\n"; }
    Test(const Test&) { cout << "Test(const Test&)\n"; }
    Test(Test&&) { cout << "Test(Test&&)\n"; }
};


int main()
{
#if 0
    compare(1, 2); // 模板实参推演
    compare("aaa", "ddd"); // 编译器会直接调用非模板函数
    compare<const char *>("aaa", "ddd"); // 调用特例化
#endif
    /* 测试 Vector  */
    Test a1, a2, a3;
    cout << "--------------------\n";
    Vector<Test> vec;
    vec.push_back(a1);
    vec.push_back(a2);
    vec.push_back(a3);
    cout << "--------------------\n";
    vec.pop_back();
    cout << "--------------------\n\n";
    
    /* 对比 push_back 和 emplace_back */
    Test t1(10);
    Vector<Test> v(100);

    cout << "==========\n";
    v.push_back(t1);
    v.emplace_back(t1);

    cout << "==========\n";
    v.push_back(Test(20));
    v.emplace_back(Test(20));

    cout << "==========\n";
    v.emplace_back(20);
    v.emplace_back(30, 40);

    return 0;
}
