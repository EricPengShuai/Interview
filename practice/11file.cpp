//
// Created by pengshuai on 2022/4/23.
//

# include<iostream>
# include<iomanip>
# include<fstream>
using namespace std;

void func1();
void func2();
void func3();
void func4();

int main()
{
    func4();
    return 0;
}


void func1()
{
    char stu[5][10];
    int i;
    for(i = 0; i < 5; i ++)
        cin.getline(stu[i],10,',');
    for(i = 0; i < 5; i ++)
        cout<< stu[i] << endl;
}


void func2()
{
    char c;
    char a[50] = "this is a string...";
    while((c=cin.get())!='\n'){
        cout.put(c);
        cout.put('\n').put('t').put('h').put('i').put('s').put('\n');
        cout.write(a,12).put('\n');
        cout<<"look"<<"\t here!"<<endl;
    }
}


/**
this is string****************----L1
****************this is string----L2
**********************-1234.90----L3
******************-1234.897600----L4
**************************0x64----L5
 **/ 
void func3()
{
    char c[30] = "this is string";
    double d = -1234.8976;
    cout << setw(30) << left << setfill('*') << c << "----L1" << endl;
    cout << setw(30) << right << setfill('*') << c << "----L2" << endl;
    // showbase显示数值的基数前缀
    cout << dec << showbase << showpoint << setw(30) << d << "----L3" << "\n";
    // showpoint显示小数点
    cout << setw(30) << showpoint << setprecision(10) << d << "----L4" << "\n";
    // setbase(8)设置八进制
    cout << setw(30) << setbase(16) << 100 << "----L5" << "\n";
}


void func4()
{
    fstream ioFile;
    ioFile.open("./a.dat", ios::out);
    ioFile << "张三" << "  " << 76 << " " << 98 << " " << 67 << endl; //L3
    ioFile << "李四" << "  " << 89 << " " << 70 << " " << 60 << endl;
    ioFile << "王十" << "  " << 91 << " " << 88 << " " << 77 << endl;
    ioFile << "黄二" << "  " << 62 << " " << 81 << " " << 75 << endl;
    ioFile << "刘六" << "  " << 90 << " " << 78 << " " << 67 << endl;
    ioFile.close();

    ioFile.open("./a.dat", ios::in|ios::binary);
    char name[10];
    int chinese, math, computer;
    cout << "姓名\t" << "英语\t" << "数学\t" << "计算机\t" << "总分" << endl; 
    ioFile >> name;                                       
    while(!ioFile.eof()) {                               
        ioFile >> chinese >> math >> computer;                    
        cout << name << "\t" << chinese << "\t" << math << "\t" << computer << "\t" << chinese+math+computer << endl;       
        ioFile >> name;
    }
    ioFile.close(); 
}
