#include <iostream>
#include <queue>
#include <thread>
#include <memory>
#include <condition_variable>

using namespace std;

std::mutex mtx; // 互斥锁实现线程之间的互斥操作
std::condition_variable cv; // 条件变量实现线程之间通信操作

class Queue {
  public:
    void put(int val) {
        unique_lock<std::mutex> lck(mtx);
        if (q.size() == 10) {
            // 生产者队列满了就应该通知消费者消费
            // 生产者线程应该进入 #1 等待状态，并且 #2 把 mtx 释放掉
            cv.wait(lck);
        }
        q.push(val);

        /**
         * @brief 通知所有线程 notify_all()，通知一个线程 notify_one()
         * 通知其他所有的线程，我生产了一个物品你们赶紧地去消费
         * 其他线程得到了该通知就会从等待状态 => 阻塞状态 => 获取互斥锁之后才能继续执行
         */
        cv.notify_all();
        cout << "生产者 生产: " << val << "号物品\n";
    }

    void get() {
        unique_lock<std::mutex> lck(mtx);
        //!NOTE: 这里写成 while 是为了防止 cv.wait 被虚假唤醒
        while (q.empty()) {
            // 消费者线程发现队列为空就需要通知生产者线程先生产物品
            // #1 进入等待状态 #2 释放 mtx
            cv.wait(lck);
        }
        int val = q.front();
        q.pop();

        // 通知其他所有的线程，我消费了一个物品你们赶紧地去生产
        cv.notify_all();
        cout << "消费者 消费: " << val << "号物品\n";
    }

  private:
    queue<int> q;
};

void producer(Queue *q)  // 生产者线程
{
    for (int i = 1; i <= 10; ++i) {
        q->put(i);
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

void consumer(Queue *q)  // 消费者线程
{
    for (int i = 1; i <= 10; ++i) {
        q->get();
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

int main() {
    Queue q;

    std::thread t1(producer, &q);
    std::thread t2(consumer, &q);

    t1.join();
    t2.join();

    return 0;
}
