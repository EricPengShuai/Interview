#include<iostream>
using namespace std;

struct student
{
    int num;
    char name[20];
    char gender;
};

int main()
{
    student s = {10, "asd", 'M'};
    cout << sizeof(s.num) << endl;      // 4
    cout << sizeof(s.name) << endl;     // 10
    cout << sizeof(s.gender) << endl;   // 1
    cout << sizeof(s) << endl;          // 4 + 20 + 4 字节对齐
    return 0;
}
