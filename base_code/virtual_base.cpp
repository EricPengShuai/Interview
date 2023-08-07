/* 虚基类 */

#include <iostream>
using namespace std;

class A
{
public:
    virtual void show() {
        cout << "A::show()" << endl;
    }

    void operator delete(void *ptr) {
        cout << "A::opeartor delete p: " << ptr << endl;
        free(ptr);
    }

private:
    int ma;
};

class B : virtual public A
{
public:
    void show() {
        cout << "B::show()" << endl;
    }

    void* operator new(size_t size) {
        void *p = malloc(size);
        cout << "operator new p: " << p << endl;
        return p;
    }

    void operator delete(void *ptr) {
        cout << "B::opeartor delete p: " << ptr << endl;
        free(ptr);
    }
private:
    int mb;
};


int main() 
{
    /** @brief 基类指针指向派生类对象，永远指向的都是派生类基类部分数据的起始地址
     * 1. windows 下 VS 编译器会有问题，delete p 从 vfptr 开始，但是 new 的其实地址是 vbptr
     * 2. Linux 下 g++ 编译器没有问题，delete p 会从 vbptr 开始释放内存
     * 
     * B 的内存布局
     * +---
     * | {vbptr} virtual base ptr
     * | mb
     * +---
     * +--- (virutal base A)
     * | {vfptr} virtual function ptr
     * | ma
     * +---
     * 
     * B::$vbtable@:
     * | 0 (vbptr 起始偏移量，因为在最上面所以偏移量为 0)
     * | 8
     * 
     * B::$vftable@:
     * | -16 (vptr 起始偏移量，vptr 上面有`vbptr`--8字节和`mb`--4字节，对齐为 8 字节，所以偏移量为 -16)
     * | &B::show
     * */
    A *p = new B();
    cout << "A *p " << p << endl;
    p->show();
    delete p;

    return 0;
}
