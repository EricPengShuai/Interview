#include <iostream>
#include <string>
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
    return 0;
}