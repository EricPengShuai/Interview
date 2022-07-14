#include <queue> 
#include <chrono> 
#include <mutex> 
#include <thread> 
#include <iostream> 
#include <condition_variable>
using namespace std;

int main() {
    queue<int> produced_nums; 
    mutex mtx; 
    condition_variable cv; 
    bool notified = false; // 通知信号

    // 生产者
    auto producer = [&]() {
        for (int i = 0; ; i++) {
            this_thread::sleep_for(chrono::milliseconds(900)); 
            unique_lock<mutex> lock(mtx); 
            cout << "producing " << i << endl; 
            produced_nums.push(i); 
            notified = true; 
            cv.notify_all(); // 此处也可以使用 notify_one
        }
    };

    // 消费者 
    auto consumer = [&]() {
        while (true) { 
            unique_lock<mutex> lock(mtx); 
            while (!notified) { // 避免虚假唤醒 
                cv.wait(lock); 
            } 
            // 短暂取消锁，使得生产者有机会在消费者消费空前继续生产 
            lock.unlock(); 
            this_thread::sleep_for(chrono::milliseconds(1000)); // 消费者慢于生产者 
            lock.lock(); 
            while (!produced_nums.empty()) { 
                cout << "consuming " << produced_nums.front() << endl;
                produced_nums.pop();
            } 
            notified = false;
        }
    };

    // 分别在不同的线程中运行
    thread p(producer); 
    thread cs[2];
    for (int i = 0; i < 2; ++i) {
        cs[i] = thread(consumer);
    } 
    p.join();
    for (int i = 0; i < 2; ++i) {
        cs[i].join();
    } 
    return 0;
}

