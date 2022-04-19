#include <iostream>
using namespace std;
class Time{
private:
    int hh,mm,ss;
public:
    explicit Time(int h=0, int m=0, int s=0): hh(h),mm(m),ss(s) {}

    // 重载 =
    Time& operator = (const Time& t) {
        hh = t.hh;
        mm = t.mm;
        ss = t.ss;
        return *this;
    }

    // 重载 ()
    void operator()(int h,int m,int s) {
        hh = h;
        mm = m;
        ss = s;
    }

    // 重载 []
    int operator[] (int i) { 
        cout << "X::operator[" << i << "]" << endl; 
        return i; 
    };
    int operator[] (const char * cp) { 
        cout << "X::operator[" << cp << "]" << endl; 
        return 0; 
    };

    // 友元重载需要参数
    friend Time operator--(Time &t);
    friend Time operator--(Time &t, int);

    // 重载二元运算符
    Time operator+ (Time t) const {
        return Time(hh + t.hh, mm + t.mm, ss + t.ss);
    }
    Time operator- (Time t) const {
        return Time(hh - t.hh, mm - t.mm, ss - t.ss);
    }


    void ShowTime() const {
        cout << hh << ":" << mm << ":" << ss << endl;
    }
};

Time operator--(Time &t) {
    -- t.ss;
    return t;
}

Time operator--(Time &t, int) {
    t.ss --;
    return t;
}

int main() {
    Time t1(12,10,11);
    t1.ShowTime();

    cout << "reload () method1:\n";
    // 第一种写法
    t1.operator()(23,20,34);	
    t1.ShowTime();

    cout << "reload () method2:\n";
    // 第二种写法
    t1(10,10,10);               	
    t1.ShowTime();
    cout << endl;

    cout << "reload = method:\n";
    Time t2;
    t2 = t1;
    t2.ShowTime();
    cout << endl;

    cout << "reload [] method:\n";
    int a = t2[1]; 
    int b = t2["abcd"];
    cout << "a=" << a << endl;
    cout << "b=" << b << endl << endl;

    cout << "reload --t and t--:\n";
    -- t2;
    t2.ShowTime();
    t2 --;
    t2.ShowTime();
    cout << endl;

    cout << "reload + and -:\n";
    Time t3 = t1 + t2;
    t3.ShowTime();
    Time t4 = t1 - t2;
    t4.ShowTime();
    return 0;
}



    
  

