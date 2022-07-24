#include <iostream>
#include <memory>
using namespace std;


/* 
unique_ptr 是 C++11 提供的用于防止内存泄漏的智能指针中的一种实现，独享被管理对象指针所有权的智能指针。
unique_ptr 对象包装一个原始指针，并负责其生命周期。当该对象被销毁时，会在其析构函数中删除关联的原始指针。
unique_ptr 具有 -> 和 * 运算符重载符，因此它可以像普通指针一样使用。
*/

struct Task {
    int mId;
    Task(int id ) :mId(id) {
        cout << "Task::Constructor" << id << endl;
    }
    ~Task() {
        cout << "Task::Destructor" << this->mId << endl;
    }
};

int main()
{
    // 通过原始指针创建 unique_ptr 实例
    // 不能通过赋值创建对象，也不能复制
    // unique_ptr<Task> taskPtr2 = new Task(1); // 编译错误
    unique_ptr<Task> taskPtr(new Task(23));

    //通过 unique_ptr 访问其成员
    int id = taskPtr->mId;
    cout << "taskPtr():" << id << endl;


    // 创建一个空的unique_ptr对象
    unique_ptr<int> ptr1;
    if (!ptr1) {
        cout << "ptr1 is empty" << endl;
    }

    // 使用 std::make_unique 创建 unique_ptr 对象 | C++14
    unique_ptr<Task> ptr2 = make_unique<Task>(34);
    cout <<  "make_unique:" << ptr2->mId << endl;

    // 使用get()函数获取管理对象的指针
    Task *p2 = taskPtr.get();
    cout << "get():" << p2->mId << endl;


    return 0;
}