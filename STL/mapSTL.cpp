#include <iostream>
#include <map>
#include <string>

using namespace std;

/* 
map以模板(泛型)方式实现，可以存储任意类型的数据，包括使用者自定义的数据类型。Map主要用于资料一对一映射(one-to-one)的情況，map內部的实现自建一颗红黑树，
第一个可以称为关键字(key)，每个关键字只能在map中出现一次；
第二个可能称为该关键字的值(value)； 

begin()         返回指向map头部的迭代器
clear(）        删除所有元素
count()         返回指定元素出现的次数
empty()         如果map为空则返回true
end()           返回指向map末尾的迭代器
equal_range()   返回特殊条目的迭代器对
erase()         删除一个元素
find()          查找一个元素
get_allocator() 返回map的配置器
insert()        插入元素
key_comp()      返回比较元素key的函数
ower_bound()    返回键值>=给定元素的第一个位置
max_size()      返回可以容纳的最大元素个数
rbegin()        返回一个指向map尾部的逆向迭代器
rend()          返回一个指向map头部的逆向迭代器
size()          返回map中元素的个数
swap()          交换两个map
upper_bound()   返回键值>给定元素的第一个位置
*/

int main() {
    map<int, string> mapStu;
    // 插入元素
    mapStu.insert(pair<int, string>(000, "stu_zero"));
    mapStu.insert(map<int, string>::value_type(001, "stu_one"));
    mapStu[002] = "stu_two";

    // 查找元素
    map<int, string>::iterator iter = mapStu.find(001);
    if (iter != mapStu.end()) {
        cout << "001 is found: " <<iter->second << endl;
    } else {
        cout << "001 is not found!" << endl;
    }

    // 删除元素
    map<int, string>::iterator iter1 = mapStu.find(002);
    mapStu.erase(iter1);

    if (mapStu.erase(001) == 1) {
        cout << "001 --> value is deleted\n";
    }  else {
        cout << "001 -->value isn't deleted\n";
    }

    // 输出map
    for (map<int, string>::iterator i = mapStu.begin(); i != mapStu.end(); ++ i) {
        cout << i->first << ' ' << i->second << endl;
    }

    return 0;
}
