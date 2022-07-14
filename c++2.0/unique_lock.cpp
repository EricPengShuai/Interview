#include <iostream> 
#include <thread>
using namespace std;

int v = 1;

void critical_section(int change_v) {
    static mutex mtx; 
    unique_lock<mutex> lock(mtx); 
    // 执行竞争操作 
    v = change_v; 
    cout << v << endl; 
    // 将锁进行释放 
    lock.unlock();

    // 在此期间，任何人都可以抢夺 v 的持有权

    // 开始另一组竞争操作，再次加锁
    lock.lock();
    v += 1;
    cout << v << endl;
}

int main() { 
    thread t1(critical_section, 2), t2(critical_section, 3); 
    t1.join(); 
    t2.join(); 
    return 0; 
}
