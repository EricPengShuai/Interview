#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
using namespace std;

bool cmp(int a, int b)
{   
    // return a > b;   // 从大到小排序
    return a < b;   // 从小到大排序
}

static bool cmp1(vector<int> a, vector<int> b)
{   
    if (a[0] == b[0]) {
        return a[1] < b[1];
    } else {
        return a[0] < b[0];
    }
}

void display(vector<int> a) 
{
    for(int v: a) {
        cout << v << ' ';
    }
    cout << endl;
}

int main()
{
    /* vector<int> vt {1, -1, 0, 4};
    display(vt);
    
    // 倒叙排列
    sort(vt.begin(), vt.end(), greater<int>());
    display(vt);
    
    // 顺序排列
    sort(vt.begin(), vt.end(), less<int>());
    display(vt);

    vector<int> a {2, 4, 1, 5};
    // 自定义排序cmp
    sort(a.begin(), a.end(), cmp);
    display(a); */

    vector< vector<int> > b(4);
    // srand(time(0));
    
    b[3].push_back(1);
    b[3].push_back(2);
    b[2].push_back(1);
    b[2].push_back(3);
    b[1].push_back(2);
    b[1].push_back(4);
    b[0].push_back(3);
    b[0].push_back(4);

    // 使用自定义的cmp，如果报错就用static
    // sort(b.begin(), b.end(), cmp1);

    // 使用lambda表达式
    sort(b.begin(), b.end(), [](vector<int> a, vector<int> b){
        if (a[0] == b[0]) {
            return a[1] < b[1];
        } else {
            return a[0] < b[0];
        }
    });
    
    for(vector<vector<int> >::iterator it = b.begin(); it != b.end(); ++ it) {
        for(unsigned long i = 0; i < (*it).size(); ++ i) {
            cout << (*it)[i] << ' ';
        }
        cout << endl;
    }
    return 0;
}