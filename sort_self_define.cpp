/* 
原数组
6 1 1 5 8 2 3 0 4

处理之后：奇数在前顺序，偶数在后逆序
1 1 3 5 8 6 4 2 0

自定义sort几种方法：
1. 模板
2. 函数
3. 逆序greater<int> 顺序less<int>
4. lambda表达式：重点掌握
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 自定义sort 1.使用模板
template <typename T>
struct cmp1
{
    bool operator()(const T &x, const T &y)
    {
        return x > y;
    }
};

// 自定义sort 2.使用函数
int cmp2(int x, int y) {
    return x > y;
}

int main()
{
    vector<int> vt = {6, 2, 1};
    int left = 0;
    int right = vt.size() - 1;
    while(left <= right) {
        while (left <= right && vt[left] % 2 == 1) {
            ++ left;
        }
        while (left <= right && vt[right] % 2 == 0) {
            -- right;
        }
        if (left > right) {
            break;
        }
        swap(vt[left], vt[right]);
    }
    cout << "排序之前" << endl;
    for (auto v: vt) {
        cout << v << ' ';
    }
    cout << endl;

    sort(vt.begin(), vt.begin()+left);
    sort(vt.begin()+left, vt.end(), cmp1<int>());
    // 逆序greater 顺序less
    // sort(vt.begin()+left, vt.end(), greater<int>());
    
    // 自定义sort 3.使用lambda表达式
    /*sort(vt.begin()+left, vt.end(), [](const int &x, const int &y) {
        return x > y;
    });*/
    cout << "排序之后" << endl;
    for (auto v: vt) {
        cout << v << ' ';
    }
    cout << endl;
    return 0;
}