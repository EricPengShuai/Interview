#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
using namespace std;

/*
// left trim c++11弃用 c++17移除
inline string& ltrim(string &str) {
    string::iterator p = find_if(str.begin(), str.end(), not1(ptr_fun<int, int>(isspace)));
    str.erase(str.begin(), p);
    return str;
}
 
// right trim c++11弃用 c++17移除
inline string& rtrim(string &str) {
    string::reverse_iterator p = find_if(str.rbegin(), str.rend(), not1(ptr_fun<int , int>(isspace)));
    str.erase(p.base(), str.end());
    return str;
}
 
// trim left and right c++11弃用 c++17移除
inline string& trim(string &str) {
    ltrim(rtrim(str));
    return str;
}
*/


string LTrim(const string& str) 
{ 
    return str.substr(str.find_first_not_of(" \n\r\t")); 
} 

string RTrim(const string& str) 
{
    return str.substr(0,str.find_last_not_of(" \n\r\t")+1); 
} 

string Trim(const string& str) 
{ 
    return LTrim(RTrim(str)); 
}


int main()
{
    /* 
    string相关：初始化字符串为空 && 其他初始化方法
        string s;
        string s1(s);
        string s2(4,'c');
     */
    
    string s = "woshihsui";

    // string find方法查看某个字符或者字符串
    if (s.find("shi") != s.npos) {
        cout << "\"shi\" first appear in s: " << s.find("shi") << endl;
    } else {
        cout << "can't find \"shi\" " << endl;
    }
    if (s.find('s') != s.npos) {
        cout << "\'s\' first apper in s: " << s.find('s') << endl;
    } else {
        cout << "'s' not in s" << endl;
    }

    // substr方法
    cout << "s[2:]" << s.substr(2) << endl;
    cout << "s[:2]" << s.substr(0, 2) << endl;

    // trim 函数例子 
    string sp = "  blue the sky ";
    cout << "left trim:" << LTrim(sp).length() << endl;
    cout << "right trim:" << RTrim(sp).length() << endl;
    cout << "trim:" << Trim(sp).length() << endl;


    /*
    erase函数的原型如下：
    （1）string& erase ( size_t pos = 0, size_t n = npos );
    （2）iterator erase ( iterator position );
    （3）iterator erase ( iterator first, iterator last );
    也就是说有三种用法：
    （1）erase(pos,n); 删除从pos开始的n个字符，比如erase(0,1)就是删除第一个字符
    （2）erase(position);删除position处的一个字符(position是个string类型的迭代器)
    （3）erase(first,last);删除从first到last之间的字符（first和last都是迭代器）
    */
    s.erase(s.begin());
    cout << s << endl;


    /*
    <sstream> 定义了三个类：
    istringstream、ostringstream 和 stringstream，分别用来进行流的输入、输出和输入输出操作.
    参考: https://urlify.cn/vMrYzm
    */
    stringstream sstream;
    // 输入到stringstream
    sstream << "first  string," << " second string";
    cout << "strResult is: " << sstream.str() << endl;

    // good(): checks if no error has occurred i.e. I/O operations are available
    cout << "stringstream.good(): " << sstream.good() << endl;
    
    // 清空sstream
    sstream.str("");
    sstream << "third string";
    cout << "After clear, strResult is: " << sstream.str() << endl;

    // 类型转换
    char c;
    int num1, num2;
    sstream.str("");
    sstream << "1.002";
    cout << sstream.str() << endl;
    sstream >> num1 >> c >> num2;
    cout << "输入输出转换：" << num1 << ' ' << c << ' ' << num2 << ' ' << sstream.good() << endl;
    return 0;
}