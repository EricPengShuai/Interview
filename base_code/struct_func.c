//
// Created by pengshuai on 2022/3/24.
//

#include<stdio.h>

struct Base {
    int v1;
//    public:       //error
    int v2;
//    private:      //error
    int v3;
    /*void print(){       // error! c中不能在结构体中嵌入函数
        printf("%s\n","hello world");
    };*/
};

void Base(){
    printf("%s\n","I am Base func");
}

// Base base2; //error

int main() {
    struct Base base;
    base.v1 = 1;
    printf("%d\n",base.v1);
    Base();
    return 0;
}