#include <iostream>
#include <queue>

using namespace std;
int main()
{
    // 默认是大顶堆，priority_queue<int, vector<int>, less<int> > a
    priority_queue<int> a;

    // 小顶堆
    priority_queue<int, vector<int>, greater<int> > c;
    priority_queue<string> b;

    // 插入元素
    for (int i = 0; i < 5; ++ i) {
        a.push(i);
        c.push(i);
    }

    // 打印元素，注意优先顺序
    while(!a.empty()) {
        cout << a.top() << ' ';
        a.pop();
    }
    cout << endl;

    while(!c.empty()) {
        cout << c.top() << ' ';
        c.pop();
    }
    cout << endl;

    b.push("abc");
    b.push("abcd");
    b.push("cbd");
    b.emplace("aaa");
    while(!b.empty()) {
        cout << b.top() << ' ';
        b.pop();
    }
    cout << endl;
    return 0;
}
/*
优先队列具有队列的所有特性，包括队列的基本操作，只是在这基础上添加了内部的一个排序，它本质是一个堆实现的。

和队列基本操作相同:

top 访问队头元素
empty 队列是否为空
size 返回队列内元素个数
push 插入元素到队尾 (并排序)
emplace 原地构造一个元素并插入队列
pop 弹出队头元素
swap 交换内容
定义：priority_queue<Type, Container, Functional>
*/