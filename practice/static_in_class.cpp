#include<iostream>
using namespace std;

class A
{
private:
    int a;
    static int sa;
public:
    void show()
    {
        cout << "show(): " << sa++ << endl;
    }
    static void s_show()
    {
        //cout << a << endl; //报错，静态成员函数不能调用非静态成员变量。无法使用this.a
        cout << "s_show(): " << sa << endl;
        // show();  //报错，静态成员函数不能调用非静态成员函数。无法使用this.show()
    }
};
int A::sa = 1;  //只在定义时可以不受private的限制，可以用类名调用私有成员。其他时候不行
                //若不赋初值，则默认初始化赋值0

int main()
{
    /*
    输出结果为4，并不是8，因为static成员变量不占用类的大小
    在类中只是声明，而在类外定义并分配内存 
    */
    cout << "sizeof(A): " << sizeof(A) << endl;      
    
    // cout << A::sa << endl;        //sa是private私有成员，不能用类名来访问
    A ca;
    ca.show();
    A::s_show();
}
