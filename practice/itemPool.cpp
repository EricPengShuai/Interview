#include <iostream>
using namespace std;

template<typename T>
class Queue
{
public:
    Queue()
    {
        _front = _rear = new QueueItem();
    }
    ~Queue()
    {
        QueueItem *cur = _front;
        while (cur != nullptr)
        {
            _front = _front->_next;
            delete cur;
            cur = _front;
        }

    }

    void push(const T &val) // 入队操作
    {
        QueueItem *item = new QueueItem(val);
        _rear->_next = item; // 类似于尾插法
        _rear = item;
    }

    void pop() // 出队操作
    {
        if (empty())
            return;
        QueueItem *first = _front->_next;
        _front->_next = first->_next;
        if (_front->_next == nullptr) {
            _rear = _front;
        }
        delete first;
    }

    T front() const
    {
        return _front->_next->_data;
    }

    bool empty() const { return _front == _rear; }

private:
    // 产生一个 QueueItem 的对象池（10000 个 QueueItem 节点）
    struct QueueItem
    {
        QueueItem(T data = T()) : _data(data), _next(nullptr) {}
        
        // 给 QueueItem 提供自定义的内存管理
        void *operator new(size_t size)
        {
            if (_itemPool == nullptr)
            {
                _itemPool = (QueueItem*) new char[POOL_ITEM_SIZE * sizeof(QueueItem)];
                QueueItem *p = _itemPool;
                for (; p < _itemPool + POOL_ITEM_SIZE - 1; ++ p) {
                    p->_next = p + 1;
                }
                p->_next = nullptr;
            }

            QueueItem *p = _itemPool;
            _itemPool = _itemPool->_next;
            return p;
        }

        void operator delete(void *ptr)
        {
            QueueItem *p = (QueueItem*)ptr;
            p->_next = _itemPool; // 相当于接到链表头
            _itemPool = p;
        }

        T _data;
        QueueItem *_next;
        static QueueItem *_itemPool; // 指向对象池首个未分配的节点
        static const int POOL_ITEM_SIZE = 10000;
    };

    QueueItem *_front;  // 指向头节点的前一个？
    QueueItem *_rear;   // 指向队尾
};

template<typename T>
typename Queue<T>::QueueItem *Queue<T>::QueueItem::_itemPool = nullptr;

int main() 
{
    Queue<int> que;
    for (int i = 0; i < 10000; ++ i) {
        que.push(i);
        que.pop();
    }
    cout << que.empty() << endl;

    return 0;
}
