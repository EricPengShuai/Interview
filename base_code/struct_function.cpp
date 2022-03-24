//
// Created by pengshuai on 2022/3/24.
//

#include<iostream>
#include<cstdio>
using namespace std;

struct Base {
    int v1{};
private:
    int v3{};
public:     // 显示声明public
    int v2{};
    static void print(){
        printf("%s\n","Base hello world");
    };
    Base() {
        cout<<"Base construct"<<endl;
    };
    virtual ~Base() {
        cout<<"Base deconstruct"<<endl;
    };
};

typedef struct Base1 {
    int v1;
//    private:   //error!
    int v3;
public:     //显示声明public
    int v2;
    static void print(){
        printf("%s\n","hello world");
    };
} B;

struct Derived:Base {

    Derived() {
        cout<<"Derived construct"<<endl;
    };
    ~Derived() override {
        cout<<"Derived deconstruct"<<endl;
    };
public:
    int v2 {};
    static void print(){
        printf("%s\n","Derived, hello world!");
    };
};


//void Base(){
//    printf("%s\n","I am Base func");
//}
//void B() {}  //error! 符号 "B" 已经被定义为一个 "struct Base1" 的别名

int main() {
//    Base base{};  // error
    struct Base base{};
    base.v1=1;
//    base.v3=2;    // error
    Base::print();
    printf("%d\n",base.v1);
//    printf("%d\n",base.v3);   // error
    Base *b = new Derived();
    b->print(); // todo：Base::print() 和 class 有点不一样
    delete b;
    return 0;
}