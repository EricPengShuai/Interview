#include <iostream> 
#include <thread>
using namespace std;

int v = 1;


void critical_section(int change_v) {
    static mutex mtx; 
    lock_guard<mutex> lock(mtx);

    // 执行竞争操作
    v = change_v;

    // 离开此作用域后 mtx 会被释放
}


int main() {
    thread t1(critical_section, 2), t2(critical_section, 3); 
    t1.join(); 
    t2.join();

    cout << v << endl; 
    return 0;
}
