#include <iostream>
#include <list>
#include <forward_list>

using namespace std;

template<class T>
void Print(const list<T>& my)
{
    typename list<T>::const_iterator it = my.begin();
    for (; it != my.end(); it++) {
        cout << *it << ' ';
    }
    cout << endl;
}

void Print_f(forward_list<int>& test)
{
    for(auto iter = test.cbegin(); iter != test.cend(); ++ iter) {
        cout << *iter << ' ';
    }
    cout << endl;
}

int main()
{
    // list
    list<int> list1 = {12, 23, 34};
    list<int> list2{11, 22, 33};

    auto it = list1.begin();    
    ++ it;  // points to 23

    // splice 可以把列表的某些元素 转移 到指定位置
    list1.splice(it, list2, list2.begin());
    Print(list1);   // 12 11 23 34
    Print(list2);   // 22 33

    list1.splice(list1.begin(), list1, it, list1.end());    // it points to 23
    Print(list1);   // 23 34 12 11

    list2.push_front(44);


    // forward_list
    forward_list<int> test = {1, 2, 3, 4, 5};
    forward_list<int> test2{11, 22, 33};
    test.push_front(6);
    test.insert_after(test.begin(), 7);
    Print_f(test);  // 6 7 1 2 3 4 5

    auto it_f = test.begin();
    advance(it_f, 2);   // it point to 1
    test.splice_after(test.begin(), test, it_f);    // tranfer 2 ?
    Print_f(test);  // 6 2 7 1 3 4 5

    test.splice_after(test.begin(), test2);
    Print_f(test);  // 6 11 22 33 2 7 1 3 4 5

    return 0;
}
