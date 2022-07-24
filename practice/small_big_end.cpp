#include <iostream>

using namespace std;

int main() {
	// 联合中只能有一个数据成员可以有值
	// https://blog.csdn.net/hou09tian/article/details/80816445
    union {
        short s;
        char c[sizeof(short)];
    } un;

    // https://blog.csdn.net/weixin_43886592/article/details/106336154
    // 大端（数据高位在低地址）01 02
    // 小端（数据低位在低地址）02 01
    un.s = 0x0102;

    if (sizeof(short) == 2) {
        if (un.c[0] == 1 && un.c[1] == 2) {
            cout << "大端" << endl;
        } else if (un.c[1] == 1 && un.c[0] == 2) {
            cout << "小端 " << un.s << endl;
        } else {
            cout << "unknown" << endl;
        }
    } else {
        cout << "sizeof(short)=" << sizeof(short) << endl;
    }
    return 0;
}