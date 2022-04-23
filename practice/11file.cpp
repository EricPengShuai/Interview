//
// Created by pengshuai on 2022/4/23.
//

#include<iostream>
using namespace std;

int main()
{
    char stu[5][10];
    int i;
    for(i = 0; i < 5; i ++)
        cin.getline(stu[i],10,',');
    getchar();
    for(i = 0; i < 5; i ++)
        cout<< stu[i] << endl;

    char c;
    char a[50] = "this is a string...";
    // cout << "use get() input char:";
    while((c=cin.get())!='\n'){
        cout.put(c);
        cout.put('\n');
        cout.put('t').put('h').put('i').put('s').put('\n');
        cout.write(a,12).put('\n');
        cout<<"look"<<"\t here!"<<endl;
    }

    return 0;
}