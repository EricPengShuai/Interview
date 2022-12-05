#include <iostream>
#include <vector>
#include <utility>

using namespace std;

/* 
C++11 左值 右值 https://blog.csdn.net/xiaolewennofollow/article/details/52559306
https://www.zhihu.com/question/387522517
 */
int main()
{
    vector<vector<int> > s;

    s.push_back(vector<int>{2,1});
    // s.push_back<vector<int>>({2,1}); // error

    s.emplace_back(2,3);    // 注意这里是 push {3, 3} 进去

    s.emplace_back(vector<int>{3,2,1});
    // s.emplace_back(vector<int>(3,2,1)); // error

    s.emplace_back<vector<int>>({1,2,3});
    // s.emplace_back({1,2,3});         // error
    int i = 0;
    for(auto vt: s) {
        cout << "vt[" << i << "]" << ' ';
        for(auto v: vt) {
            cout << v << ' ';
        }
        ++ i;
        cout << endl;
    }
    cout << endl;

    vector<pair<int, int>> vt;
    vt.emplace_back(3,2);                       // pair<int,int>(3,2)

    vt.emplace_back(pair<int,int>(4,2));        // pair<int,int>(4,2)
    vt.emplace_back(pair<int,int>{5,2});        // pair<int,int>(5,2)

    // vt.emplace_back(make_pair<int,int>{6,2});   // error
    vt.emplace_back(make_pair<int,int>(7,2));   // pair<int,int>(7,2)
    for(auto v: vt) {
        cout << v.first << ' ' << v.second << endl;
    }
    return 0;
}