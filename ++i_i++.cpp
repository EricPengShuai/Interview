#include <iostream>
using namespace std;

class Point{
    int x_;
    int y_;
public:
    Point(int x = 0, int y = 0) {
        x_ = x;
        y_ = y;
        cout << "this is constructor" << endl;
    }

    Point(const Point& b) {
        this->x_ = b.x_;
        this->y_ = b.y_;
        cout << "this is copy constructor" << endl;
    }

    ~Point() {
        cout << "this is destructor" << endl;
    }

    Point& operator++();            // 前置
    const Point operator++(int);    // 后置
    
    Point operator+(const Point& _right) {
        Point temp;     // a = a + b 第一次调用构造函数
        temp.x_ = this->x_ + _right.x_;
        temp.y_ = this->y_ + _right.y_;
        return temp;    // a = a + b 第二次调用构造函数
    }

    Point& operator+=(const Point& _right) {
        this->x_ += _right.x_;
        this->y_ += _right.y_;
        return *this;
    }

    void DisplayPoint();
};


Point& Point::operator++()
{
    ++x_;
    ++y_;
    return *this;
}

const Point Point::operator++(int)
{
    Point temp(*this);  // i++ 这里就需要调用构造函数
    this->x_++;
    this->y_++;
    return temp;        // i++ 这里也需要调用构造函数
}



void Point::DisplayPoint()
{
    cout << "x: " << this->x_ << endl;
    cout << "y: " << this->y_ << endl;
}

int main()
{
    Point a(1,1);
    cout << endl << "this is a++: " << endl;
    a ++;
    cout << endl << "this is ++a: " << endl;
    ++ a;

    Point b(2, 2);
    Point* c;
    cout << endl << "this is &b: " << &b << endl;

    cout << endl << "this is c = &(++b): ";
    c = &(++b);
    cout << c << endl;

    // 这里会报错，b++不能作为左值而被取地址
    // cout << endl << "this is c = &(b++): ";
    // c = &(b++);
    // cout << c << endl;
}