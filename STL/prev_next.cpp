#include <iostream>
#include <iterator>
#include <list>
using namespace std;

int main()
{
    //创建并初始化一个 list 容器
    list<int> mylist{1, 2, 3, 4, 5};
    auto it = mylist.end();
    //获取一个距离 it 迭代器 2 个元素的迭代器，由于 2 为正数，prev_it 位于 it 左侧
    auto prev_it = prev(it, 2);
    cout << "prev(it, 2) = " << *prev_it << endl;   // prev(it, 2) = 4

    // n为负数，prev_it 位于 it 右侧
    it = mylist.begin();
    prev_it = prev(it, -2);
    cout << "prev(it, -2) = " << *prev_it << endl;  // prev(it, -2) = 3

    // next
    it = mylist.begin();
    auto next_it = next(it);
    cout << "next(it) = " << *next_it << endl;   // next(it) = 2
    return 0;
}
