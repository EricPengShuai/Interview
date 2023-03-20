#include <iostream>
#include <numeric>
#include <iterator>
#include <forward_list>
#include <vector>
using namespace std;

/*
STL advance()函数: http://c.biancheng.net/view/7370.html
- 这个函数的底层实现就是it++ it--
*/
int main()
{
    vector<int> vt(5);
    iota(vt.begin(), vt.end(), 0);
    // 测试 advance
    auto itv = vt.begin();
    advance(itv, 2);
    cout << "advance(itv, 2): *itv = " << *itv << endl;
    advance(itv, -1);
    cout << "advance(itv, -1): *itv = " << *itv << endl;

    // forward_list 只支持前向迭代器
    forward_list<int> fl(5);
    iota(fl.begin(), fl.end(), 0);

    auto it = fl.begin();
    for (it = fl.begin(); it != fl.end(); ++ it) {
        cout << *it << ' ';
    }
    cout << endl;
    // 测试forward_list的迭代器 advance，不能使用advance(it, -1)
    it = fl.begin();
    advance(it, 1);

    cout << "advance(it, 1): *it = " << *it << endl;
    return 0;
}