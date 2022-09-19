#include <iostream>
#include <map>
#include <string>

using namespace std;

/* 
map以模板(泛型)方式实现，可以存储任意类型的数据，包括使用者自定义的数据类型。Map主要用于资料一对一映射(one-to-one)的情況，map內部的实现自建一颗红黑树，
第一个可以称为关键字(key)，每个关键字只能在map中出现一次；
第二个可以称为该关键字的值(value)； 

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
    mapStu.insert(pair<int, string>(0, "stu_zero"));
    
    // 这个插入无效，但是不会报错
    mapStu.insert(pair<int, string>(0, "STU_zero"));
    // 这个才会修改之前key对应的value
    mapStu[0] =  "STU_ZERO";

    mapStu.insert(map<int, string>::value_type(1, "stu_one"));
    mapStu[2] = "stu_two";
    mapStu[-1] = "test";
    mapStu[3] = "shengxu";
    // 输出map
    cout << "输出map:" << endl << "iterator-----------" << endl;
    for (map<int, string>::iterator i = mapStu.begin(); i != mapStu.end(); ++ i) {
        cout << i->first << ' ' << i->second << endl;
    }
    cout << "-----------" << endl;
    cout << "auto &[key, vlaue]-----------" << endl;
    for (auto &[key, value]: mapStu) {
        cout << key << ' ' << value << endl;
    }
    cout << "-----------" << endl;

    // 查找第一个key值大于等于2的迭代器
    auto it_lower = mapStu.lower_bound(2);
    if (it_lower != mapStu.end()) {
        cout << "第一个key大于等于2的map是" << it_lower->first << ' ' << it_lower->second << endl;
    } else {
        cout << "不存在";
    }

    // 查找第一个key值大于等于2的迭代器
    auto it_upper = mapStu.upper_bound(2);
    if (it_upper != mapStu.end()) {
        cout << "第一个key大于2的map是" << it_upper->first << ' ' << it_upper->second << endl;
    } else {
        cout << "不存在";
    }

    // count: 某个key存在就返回1，否则就返回0
    cout<<"count(1):";
    cout<<mapStu.count(1)<<endl;

    // 查找元素
    map<int, string>::iterator iter = mapStu.find(1);
    if (iter != mapStu.end()) {
        cout << "1 is found: " <<iter->second << endl;
    } else {
        cout << "1 is not found!" << endl;
    }

    // 删除元素
    map<int, string>::iterator iter1 = mapStu.find(2);
    mapStu.erase(iter1);

    if (mapStu.erase(1) == 1) {
        cout << "1 --> value is deleted\n";
    }  else {
        cout << "1 -->value isn't deleted\n";
    }
    return 0;
}
