## 弹性计算

### 计算调度团队

> 2023.02.13

介绍团队主要的业务，主要是Java/Python，C/C++ 占 1/3 

聊项目，30min

**八股**

1、数据库中的 transaction

2、Python 中的协程和线程，不太了解

- 进程(Process)：进程是计算机中的程序关于某数据集合的一次运行实例，是操作系统进行**资源分配**的最小单位
- 线程(Thread)：线程被包含在进程之中，是操作系统进行**程序调度**执行的最小单位
- 协程(Coroutine)：协程是**用户态**执行的轻量级编程模型，由单一线程内部发出控制信号进行调度

协程由单一线程内部发出控制信号进行调度，而非受到操作系统管理，因此**协程没有切换开销和同步锁机制**，具有极高的执行效率。

**协程常用于IO密集型工作**，例如网络资源请求等；而**进程、线程常用于计算密集型工作**，例如科学计算、人工神经网络等。、

> 参考：https://blog.csdn.net/FRIGIDWINTER/article/details/124369567



**算法**

1、三数之和：[15. 三数之和](https://leetcode.cn/problems/3sum/)

2、根据上排给出0,1,2 …… n个数(n >= 3)组成的等差数列 ,在其下排填出对应的n+1个数, 要求下排每个数都是先前上排对应那个数在下排数中出现的次数 , 如：

```cpp
// 1和2下面的数永远是2和1，0下面对应的数为n-3（n>=6），上排数n-3下面对应的数为1，其它上排数下面对应为0就ok了
// n = 9
0 1 2 3 4 5 6 7 8 9
6 2 1 0 0 0 1 0 0 0
// n = 10
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
16 2 1 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0
// n = 6
0 1 2 3 4 5 6
3 2 1 1 0 0 0

// n = 3
0 1 2 3
1 2 1 0  
// n = 4
0 1 2 3 4
2 1 2 0 0
// n = 5 无解
```

代码求解：只能求解 `n=3` 以及 `n>=6` 的情况，无法求解 n=4/5 的情况

```cpp
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

class NumberTB {
    int len;
    bool success;
    vector<int> top;
    vector<int> bottom;

public:
    NumberTB(int n) {
        len = n < 3 ? 3 : n+1;
        success = false;
        top = vector<int>(len, 0);
        bottom = vector<int>(len, 0);
        
        // 赋初始值 0 1 2 3 ... n
        iota(top.begin(), top.end(), 0);
    }

    vector<int> getBottom() {
        int id = 0;
        while(!success) {
            success = true;
            for (int i = 0; i < len; ++ i) {
                int frequency = getFrequency(top[i]);
                if (bottom[i] != frequency) {
                    bottom[i] = frequency;
                    success = false;
                }
            }
            cout << "第 " << id++ << " 次尝试：";
            for (auto& b: bottom) {
                cout << b << ' ';
            }
            cout << endl;
        }
        return bottom;
    }

    // 计算 num 在 bottom 中出现的次数
    int getFrequency(int num) {
        int cnt = 0;
        for (int i = 0; i < len; ++ i) {
            if (bottom[i] == num) {
                ++ cnt;
            }
        }
        return cnt;
    }
};

int main()
{
    int n;
    cin >> n;
    NumberTB tb(n);
    vector<int> p = tb.getBottom();

    for (int i = 0; i <= n; ++ i) cout << i << ' ';
    cout << endl;
    
    for (auto& x: p) cout << x << ' ';
    cout << endl;
    return 0;
}
```





