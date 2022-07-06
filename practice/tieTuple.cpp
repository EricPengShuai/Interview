// packing/unpacking tuples
#include <iostream>  
#include <tuple>     // std::tuple, std::make_tuple, std::tie
#include <set>
#include <string>
using namespace std;

int main() {
    int myint;
    char mychar;
    tuple<int, float, char> mytuple;

    mytuple = make_tuple(10, 2.6, 'a');  // packing values into tuple

    tie(myint, ignore, mychar) = mytuple;  // unpacking tuple into variables
    cout << "myint contains: " << myint << '\n';
    cout << "mychar contains: " << mychar << '\n';


    set<string> set_of_str;
    bool inserted = false;
    set<string>::iterator it;
    // insert返回值一个指向插入位置的迭代器和一个布尔值
    tie(it, inserted) = set_of_str.insert("Test");
    set_of_str.insert("Test1");
    cout << *it << endl;
    
    if (inserted) {
        cout << "Value was inserted successfully\n";
    }

    return 0;
}