#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

/********************** 函数对象 **********************/
void hello(string str)
{
    cout << str << endl;
}

int sum(int a, int b)
{
    return a + b;
}

// 先要提供一个主类模板
template<typename Fty>
class myfunction {};

// 部分特例化版本：typename... 可变参类型
template<typename R, typename... A>
class myfunction<R(A...)>
{
public:
    using PFUNC = R(*)(A...);
    myfunction(PFUNC pfunc) : _pfunc(pfunc) {}

    R operator()(A... arg)
    {
        return _pfunc(arg...);
    }

private:
    PFUNC _pfunc;
};

/********************** 绑定器 **********************/
// 打印容器元素函数模板
template<typename Container>
void showContainer(Container &con)
{
    typename Container::iterator it = con.begin();
    for (; it != con.end(); ++ it) {
        cout << *it << ' ';
    }
    cout << endl;
}

template<typename Compare, typename T>
class _mybind1st
{
public:
    _mybind1st(Compare comp, T val)
        : _comp(comp), _val(val) {}
    
    bool operator()(const T &second)
    {
        return _comp(_val, second);
    }
private:
    Compare _comp;
    T _val;
};

// 自定义绑定器，返回类型是一个函数模板，重载 operator()
template<typename Compare, typename T>
_mybind1st<Compare, T> mybind1st(Compare comp, const T &val)
{
    // 直接使用函数模板好处就是可以直接进行类型的推演
    return _mybind1st<Compare, T>(comp, val);
}

// find_if 函数模板
template<typename Iterator, typename Compare>
Iterator my_find_if(Iterator first, Iterator last, Compare comp)
{
    for (; first != last; ++ first) {
        if (comp(*first)) { // comp.operator()(*first)
            return first;
        }
    }
    return last;
}

int main()
{
    /********************** 函数对象 **********************/
    myfunction<void(string)> p1(hello);
    myfunction<int(int, int)> p2(sum);

    p1("hello world!");
    cout << p2(1,2) << endl;

    /********************** 绑定器 **********************/
    vector<int> vec;
    srand(time(nullptr));
    for (int i = 0; i < 20; ++ i) {
        vec.push_back(rand() % 100 + 1);
    }

    showContainer(vec);
    sort(vec.begin(), vec.end());
    showContainer(vec);

    /** @brief 按照顺序插入一个 70
     * greater a > b
     * less    a < b
     * 绑定器 + 二元函数对象 => 一元函数对象 */
    // auto it = find_if(vec.begin(), vec.end(), bind(less<int>(), 70));
    auto it = my_find_if(vec.begin(), vec.end(), mybind1st(less<int>(), 70));
    if (it != vec.end()) {
        vec.insert(it, 70);
    }
    showContainer(vec);

    return 0;
}
