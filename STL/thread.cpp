#include <iostream>
#include <thread>
#include <mutex>

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#define sleep(n) Sleep(1000 * (n))
#else
#include <unistd.h>
#endif

using namespace std;

mutex mtx;

void t1()  // 普通的函数，用来执行线程
{
    for (int i = 0; i < 3; ++i) {
//        lock_guard<mutex> lg(mtx);
        cout << "t1 111\n";
        sleep(1);
    }
}

void t2()
{
    for (int i = 0; i < 5; ++i) {
//        lock_guard<mutex> lg(mtx);
        cout << "t2 222\n";
        sleep(1);
    }
}

int cnt = 20;
mutex m;

void t3()
{
    while (cnt > 0) {
        lock_guard<mutex> lockGuard(m);
        // m.lock();
        if (cnt > 0) {
            --cnt;
            cout << cnt << "\tt3" << endl;
        }
        // m.unlock();
    }
}

void t4()
{
    while (cnt > 0) {
        lock_guard<mutex> lockGuard(m);
        // m.lock();
        if (cnt > 0) {
            --cnt;
            cout << cnt << "\tt4" << endl;
        }
        // m.unlock();
    }
}

int main()
{
    thread th1(t1);  // 实例化一个线程对象th1，使用函数t1构造，然后该线程就开始执行了（t1()）
    thread th2(t2);

    th1.join(); // 必须将线程join或者detach 等待子线程结束主进程才可以退出
    th2.join();
    /* join 输出结果: (不是固定的，使用 mutex 也不一定固定，因为 sleep)
     * t1 111
     * t2 222
     * t1 111
     * t2 222
     * t1 111
     * t2 222
     * t2 222
     * t2 222
     * t2 222
     * here is main
     * */

//     th1.detach();
//     th2.detach();
    /* detach 输出结果:
     * here is main
     *
     * t1 111
     * t2 222
     * */

    thread th3(t3);
    thread th4(t4);

    th3.join();    // 等待t3退出
    th4.join();    // 等待t4退出
    /* 输出结果: (全是 t3 的，如果不使用 mutex 就不会固定)
     * 19	t3
     * ...	t3
     * 0    t3
     * */

//    th3.detach();
//    th4.detach();
    /*
     * 没有输出，主线程直接 return 0
     * */

    cout << "here is main\n\n";
    return 0;
}