//
// Created by pengshuai on 2022/3/31.
//

#include <iostream>
//#define isNs1 1
#define isGlobal 2
using namespace std;
void func()
{
    cout<<"::func"<<endl;
}

namespace ns1 {
    void func()
    {
        cout<<"ns1::func"<<endl;
    }
}

namespace ns2 {
#ifdef isNs1
    using ns1::func;    /// ns1中的函数
#elif isGlobal
    using ::func; /// 全局中的函数
#else
    void func()
    {
        cout<<"other::func"<<endl;
    }
#endif
}

int main()
{
    /**
     * 这就是为什么在c++中使用了cmath而不是math.h头文件
     */
    // 会根据当前环境定义宏的不同来调用不同命名空间下的func()函数
    ns2::func();
    return 0;
}