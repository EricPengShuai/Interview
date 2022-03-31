//
// Created by pengshuai on 2022/3/31.
//

#include <iostream>
using namespace std;

int count = 0;    // 全局(::)的count

class A {
public:
    static int count;  // 类A的count (A::count)
};

// 静态变量必须在此处定义
int A::count = 5;

int main() {
    ::count = 1; // 设置全局的count为1
    A::count = 2; // 设置类A的count为2
    cout << ::count << endl;
    cout << A::count << endl;
    return 0;
}