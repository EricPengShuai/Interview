#include <iostream>
#include <vector>
#include <string>
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
    // 默认升序less<int>, 如果要倒序就用greater<int>
    multiset<int, less<int> > s;
    s.insert(12);
    s.insert(2);
    s.insert(58);
    s.insert(30);
    s.insert(70);
    s.insert(12);
    // 遍历set
    set<int>::iterator it = s.begin();
    for (it = s.begin(); it != s.end(); ++ it) {
        cout << *it << ' ';
    }
    cout << endl;
    cout << "s.max_size(): " << s.max_size() << endl;
    cout << "s.size(): " << s.size() << endl;
    cout << "s.begin(): " << *s.begin() << endl;

    // 返回指向大于或等于某值的第一个元素的迭代器
    cout << "lower_bound(12): " << *s.lower_bound(12) << endl;  // 输出12
    
    // 返回指向大于某值的迭代器
    cout << "upper_bound(12): " << *s.upper_bound(12) << endl;  // 输出30
    
    // 返回一个指向被查找到元素的迭代器
    cout << "find(12): " << *s.find(12) << endl;    // 输出12


    /* 
    set<pair<int, int>> 排序操作
    使用less<>、greater<>模板对pair< int , int >排序时，先比较pair的第一个元素，如果第一个元素相等再比较第二个元素。
    */ 
    set<pair<int, int>, less<pair<int, int>>> set_pair;
    // set<pair<int, int>> set_pair;   // 默认是less

    for (int i = 0; i < 10; ++ i) {
        set_pair.insert(pair<int, int>(i, i%3));
        // set_pair.insert(pair<int, int>{i, i%3});
        // set_pair.insert(i, i%3);
    }
    for_each(set_pair.begin(), set_pair.end(), [](auto x) {
        cout << "(" << x.first << ',' << x.second << ") ";
    });
    cout << endl;


    set<pair<int, int>, greater<pair<int, int>> > set_pair_greater;
    for (int i = 0; i < 10; i ++) {
        set_pair_greater.insert(pair<int, int>(i, i%3));
    }
    for (set<pair<int, int>, greater<pair<int, int>> >::iterator iter=set_pair_greater.begin(); iter!=set_pair_greater.end(); iter++) {
        cout << "(" << iter->first << "," << iter->second << ") ";
    }
    cout << endl;
    

    /*
    set<pair<char, int>> 排序操作 lambda 表达式自定义排序
    关于仿函数类的写法参考博客：https://blog.csdn.net/Strengthennn/article/details/110387049
    */
    string str = "icaneatglass";
    vector<int> vec(26, 0);
    for (int i = 0; i < static_cast<int>(str.length()); i++) {
        vec[str[i] - 'a'] ++;
    }
    // 使用lambda表达式自定义排序
    auto cmp = [](const pair<char, int>& a, const pair<char, int>& b) {
        return a.second == b.second ? (a.first < b.first) : (a.second < b.second);
    };

    set<pair<char, int>, decltype(cmp)> set_pair_lambda(cmp);
    // 插入数据
    for (int i = 0; i < 26; i++) {
        if (vec[i] != 0) {
            set_pair_lambda.insert(pair<char, int>((char)('a' + i), vec[i]));
        }
    }
    for (auto it : set_pair_lambda) {
        cout << "(" << it.first << "," << it.second << ")";
    }
    cout << endl;

    return 0;
}