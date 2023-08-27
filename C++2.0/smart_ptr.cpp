#include <iostream>
using namespace std;

template<typename T> 
class RefCnt
{
public:
    RefCnt(T *ptr = nullptr)
        : mptr(ptr)
    {
        if (mptr != nullptr) {
            mcount = 1;
        }
    }

    void addRef() { mcount ++; }
    int delRef() { return -- mcount; }
private:
    T *mptr;
    int mcount;
};

template<typename T>
class CSmartPtr
{
public:
    CSmartPtr(T *ptr = nullptr) : mptr(ptr) {
        mpRefCnt = new RefCnt<T>(mptr);
    }
    ~CSmartPtr() 
    {
        if (0 == mpRefCnt->delRef())
        {
            delete mptr;
            delete mpRefCnt;
            mptr = nullptr;
            mpRefCnt = nullptr;
        }
    }

    T& operator*() { return *mptr; }
    T* operator->() { return mptr; }

    CSmartPtr(const CSmartPtr<T> &src)
        : mptr(src.mptr), mpRefCnt(src.mpRefCnt)
    {
        if (mptr != nullptr)
            mpRefCnt->addRef();
    }

    CSmartPtr<T>& operator=(const CSmartPtr<T> &src)
    {
        if (this == &src)
            return *this;
        
        if (0 == mpRefCnt->delRef()) {
            delete mptr;
            delete mpRefCnt;
        }

        mptr = src.mptr;
        mpRefCnt = src.mpRefCnt;
        mpRefCnt->addRef();
        return *this;
    }

private:
    T* mptr;               // 指向资源的指针
    RefCnt<T> *mpRefCnt;   // 指向该资源引用计数对象的指针
};

int main()
{
    CSmartPtr<int> p1(new int(1));
    CSmartPtr<int> p2(p1);
    CSmartPtr<int> p3;
    p3 = p2;

    *p1 = 20;
    cout << *p2 << ' ' << *p3 << endl;
    return 0;
}
