#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void showVector(vector<int> &vt) 
{
    printf("%ld elements: ", vt.size());
    for (int item : vt) {
        cout << item << ' ';
    }
    cout << endl;
}

int main() 
{
    /* 
    vector相关：
        默认初始化，vector 为空， size 为0，适用于动态元素
    其他初始化方法
        vector<int> vt1(vt);
        vector<int> vt2(7);
        vector<int> vt3(vt.begin(), vt.end());
        vector<int> vt4(7, 4);
     */ 
    vector<int> vt;

    vt.push_back(1);
    vt.push_back(2);
    vt.__emplace_back(3);   // c++17提出
    vt.pop_back();          // 弹出最后一个元素
    

    // find指定元素 包含在头文件algorithm中
    vector<int>::iterator it = find(vt.begin(), vt.end(), 2);
    if (it != vt.end()) {
        cout << "Find(2) in " << *it << endl;
    } else {
        cout << "Can't find(2)" << endl;
    }

    // remove相关: http://c.biancheng.net/view/429.html
    vector<int> v;   // 建立一个vector<int> 用1-10填充它
    v.reserve(10);   // 这个只是改变capicity() https://blog.csdn.net/amusi1994/article/details/81106455
    for (int i = 1; i <= 10; ++i) {
        v.push_back(i);
    }
    cout << "init vector:" << endl;
    showVector(v);
    v[1] = v[3] = v[5] = 99;
    cout << "change element:" << endl;
    showVector(v);
    auto iter = remove(v.begin(), v.end(), 99);     // 删除所有等于99的元素,其实并不是
    cout << "remove(99):" << endl;
    showVector(v);
    v.erase(iter, v.end());                         // 这才是真正的删除了
    cout << "erase(99):\n";
    showVector(v);
    cout << endl;    
    return 0;
}
