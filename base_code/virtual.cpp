#include <iostream>

#include<iostream>
using namespace std;


class Employee
{
public:
    virtual void raiseSalary()
    {
        cout<<0<<endl;
    }

    virtual void promote()
    { /* common promote code */ }
};

class Manager: public Employee
{
    void raiseSalary() override
    {
        cout<<100<<endl;
    }

    void promote() override
    { /* Manager specific promote */ }
};

class Engineer: public Employee
{
    void raiseSalary() override
    {
        cout<<200<<endl;
    }

    void promote() override
    { /* Manager specific promote */ }
};


void globalRaiseSalary(Employee *emp[], int n)
{
    for (int i = 0; i < n; i++)
        emp[i]->raiseSalary();
    // according to the actual object, not according to the type of pointer
}

int main()
{
    // 虚函数的调用取决于指向或者引用的对象的类型，而不是指针或者引用自身的类型
    Employee *emp[] = {new Manager(), new Engineer};
    globalRaiseSalary(emp,2);   // 100 200
    return 0;
}
