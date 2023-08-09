#include <iostream>
using namespace std;

/* 通过 C的函数指针实现比较大小 */
template<typename T>
bool mygreater(T a, T b)
{
    return a > b;
}
template<typename T>
bool myless(T a, T b)
{
    return a < b;
}

/* 通过函数对象实现大小比较 */
template<typename T>
class Mygreater
{
public:
    bool operator()(T a, T b) { // 二元函数对象
        return a > b;
    }
};
template<typename T>
class Myless
{
public:
    bool operator()(T a, T b) { // 二元函数对象
        return a < b;
    }
};

template<typename T, typename Compare>
bool compare(T a, T b, Compare comp)
{
    return comp(a, b);
}

int main()
{
    // 函数指针没法内联调用，因为函数指针调用（传的地址）时在运行阶段才知道具体类型
    // cout << compare(1, 2, mygreater<int>) << endl;
    // cout << compare(1, 2, myless<int>) << endl;

    // 通过函数对象比较大小可以实现内联，因为这个在编译期间就可以确定
    cout << compare(1, 2, Mygreater<int>()) << endl;
    cout << compare(1, 2, Myless<int>()) << endl;

    return 0;
}
