#include <iostream>
#include <functional>
#include <vector>
#include <thread>

using namespace std;

class Thread
{
public:
    using PFUNC = function<void(int)>;
    Thread(PFUNC pfunc, int id) : _pfunc(pfunc), _no(id) {}

    thread start() {
        thread t(_pfunc, _no);
        return t;
    }

private:
    PFUNC _pfunc;
    int _no;
};

class ThreadPool
{
public:
    ThreadPool() {}
    ~ThreadPool() {
        // 释放 Thread 对象占用的堆资源
        for (int i = 0; i < _pool.size(); ++ i) {
            delete _pool[i];
        }
    }

    // 开启线程池
    void startPool(int size)
    {
        for (int i = 0; i < size; ++ i) {
            // Thread 参数是一个函数对象，可以使用 bind 绑定
            _pool.push_back(new Thread(bind(&ThreadPool::runInThread, this, std::placeholders::_1), i));
        }

        for (int i = 0; i < size; ++ i) {
            _handler.push_back(_pool[i]->start());
        }

        for (thread &t: _handler) {
            t.join();
        }
    }
private:
    vector<Thread *> _pool;
    vector<thread> _handler;

    void runInThread(int id)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout << "call runInThread! id:" << id << endl;
    }
};

int main()
{
    ThreadPool pool;
    pool.startPool(10);
    return 0;
}
