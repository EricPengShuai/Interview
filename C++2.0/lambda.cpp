//
// Created by pengshuai on 2022/6/26.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    // [] 基本的 lambda 表达式
    auto basicLambda = [] {
        cout << "Basic Lambda Express!" << endl;
    };
    basicLambda();

    // lambda 表达式带参数
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    cout << add(2, 3);

    // [] 其实就是一个闭包，用来定义捕捉模式以及变量，表示的是lambda捕捉块
    int x = 10;
    auto add_x = [x](int a) {       // 复制捕捉x
        return a + x;
    };
    auto multiply_x = [&x](int a) { // 引用捕捉x
        return a * x;
    };
    cout << add_x(10) << ' ' << multiply_x(10) << endl;

    // mutbale 关键字可以让复制捕捉方式的表示修改参数
    auto add_x_mutable = [x](int a) mutable {
        x *= 2;
        return a + x;
    };
    cout << add_x_mutable(10) << ' ' << x << endl;

    // lambda 表达式禁用了辅助操作符，但是没有禁用复制构造函数
    auto add_copy = add_x_mutable;
    cout << add_copy(10) << ' ' << x << endl;

    /*
     * 参考：https://www.jianshu.com/p/d686ad9de817
     * []：默认不捕获任何变量；
     * [=]：默认以值捕获所有变量；
     * [&]：默认以引用捕获所有变量；
     * [x]：仅以值捕获x，其它变量不捕获；
     * [&x]：仅以引用捕获x，其它变量不捕获；
     * [=, &x]：默认以值捕获所有变量，但是x是例外，通过引用捕获；
     * [&, x]：默认以引用捕获所有变量，但是x是例外，通过值捕获；
     * [this]：通过引用捕获当前对象（其实是复制指针）；
     * [*this]：通过传值方式捕获当前对象；
     *
     * 最好不要使用 [=] [&] 捕获所有变量，可能会出现悬挂引用
     *
     * */

    // lambda 表达式作为传递的参数
    int value = 3;
    vector<int> v = {1, 2, 3, 5, 4, 7};
    int count = count_if(v.begin(), v.end(), [value](int x) { return x > value; });
    cout << count << endl;

    vector<int> vt(10);
    int a = 0;
    int b = 1;
    generate(vt.begin(), vt.end(), [&a, &b] {
        int value = b;
        b = b + a;
        a = value;
        return value;
    });
    for(auto element: vt) {
        cout << element << ' ';
    }
    cout << endl;

    // lambda 捕获表达式
    int x1 = 5;
    auto y = [&r = x1, x1 = x1 + 1] {
        r += 2;
        return x1 * x1;
    };
    cout << x1 << ' ' << y() << endl;

    auto z = [str = "string"] {
        return str;
    };
    cout << z() << endl;

    // 泛型lambda表达式
    auto  add1 = [](auto x, auto y) { return x + y; };
    cout <<  add1(2, 3) << ' ' << add1(2.3, 2.3) << endl;

    // 函数对象
    for_each(vt.begin(), vt.end(), [](int x) { cout << x << ' '; });
    cout << endl;

}