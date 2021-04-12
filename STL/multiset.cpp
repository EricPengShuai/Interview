#include <iostream>
#include <set>
using namespace std;

/* 
set: https://www.cnblogs.com/caiyishuai/p/8646345.html
multiset: http://c.biancheng.net/view/545.html
1. multiset<T> 容器就像 set<T> 容器，但它可以保存重复的元素。
2. 这意味我们总可以插入元素，当然必须是可接受的元素类型。默认用 less<T> 来比较元素，但也可以指定不同的比较函数。
3. 在元素等价时，它必须返回 false。
 */

int main()
{
    // 默认升序less<int>, 如果要倒叙就用greater<int>
    set<int, less<int> > s;
    s.insert(12);
    s.insert(2);
    s.insert(58);
    s.insert(30);
    s.insert(70);
    set<int>::iterator it = s.begin();
    for (it = s.begin(); it != s.end(); ++ it) {
        cout << *it << ' ';
    }
    cout << endl;
    cout << "s.max_size(): " << s.max_size() << endl;
    cout << "s.size(): " << s.size() << endl;
    cout << "s.begin(): " << *s.begin() << endl;
    // 返回指向大于或等于某值的第一个元素的迭代器
    cout << "lower_bound(12): " << *s.lower_bound(12) << endl;
    // 返回指向大于某值的迭代器
    cout << "upper_bound(12): " << *s.upper_bound(12) << endl;
    // 返回一个指向被查找到元素的迭代器
    cout << "find(12): " << *s.find(12) << endl;
    return 0;
}