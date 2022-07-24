//
// Created by pengshuai on 2022/7/24.
//


#include <iostream>     // cout
#include <functional>   // minus
#include <numeric>      // accumulate
using namespace std;

/*
 * init 是初始量
 * template <class InputIterator, class T>
 * T accumulate (InputIterator first, InputIterator last, T init);
 *
 * binary_op 是二元运算符号，例如 minus<int>()
 * template <class InputIterator, class T, class BinaryOperation>
 * T accumulate (InputIterator first, InputIterator last, T init, BinaryOperation binary_op);
 * */


int myfunction (int x, int y) {return x+2*y;}
struct myclass {
    int operator()(int x, int y) {return x+3*y;}
} myobject;

int main () {
    int init = 100;
    int numbers[] = {10,20,30};

    cout << "using default accumulate: ";
    cout << accumulate(numbers,numbers+3, init);
    cout << '\n';

    cout << "using functional's minus: ";
    cout << accumulate (numbers, numbers+3, init, minus<int>());
    cout << '\n';

    cout << "using custom function: ";
    cout << accumulate (numbers, numbers+3, init, myfunction);
    cout << '\n';

    cout << "using custom object: ";
    cout << accumulate (numbers, numbers+3, init, myobject);
    cout << '\n';

    return 0;
}

/* 输出结果：
 * using default accumulate: 160
 * using functional's minus: 40
 * using custom function: 220
 * using custom object: 280
 * */