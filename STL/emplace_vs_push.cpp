#include <iostream>
#include <vector>
using namespace std;


/* 
C++11 左值 右值 https://blog.csdn.net/xiaolewennofollow/article/details/52559306
emplace_back VS push_back https://www.cnblogs.com/ChrisCoder/p/9919646.html
https://www.zhihu.com/question/387522517
 */
int main()
{
    
    class test
    {
        
    public:
        int x, y;
        test(int x, int y): x(x), y(y) {};
    };
    vector<vector<int> > s;

    // s.emplace_back(1,2);
    // s.emplace_back(vector<int>{3,2,1});
    // s.emplace_back<vector<int>>({1,2,3});
    s.emplace_back({1,2,3});
    cout << endl;
    return 0;
}