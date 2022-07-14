#include <iostream> 
#include <future> 
#include <thread>
using namespace std;

int main() {

    // 将一个返回值为 7 的 lambda 表达式封装到 task 中 
    // packaged_task 的模板参数为要封装函数的类型 
    // packaged_task，它可以用来封装任何可以调用的目标， 从而用于实现异步的调用
    packaged_task<int()> task([](){return 7;}); 

    // 获得 task 的期物
    future<int> result = task.get_future(); // 在一个线程中执行 task

    thread(move(task)).detach(); 
    cout << "waiting..."; 
    result.wait(); // 在此设置屏障，阻塞到期物的完成 

    // 输出执行结果 
    cout << "done!" <<  endl << "future result is " << result.get() << endl; 
    return 0;
}
