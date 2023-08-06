//
// Created by pengshuai on 2022/4/23.
//

#include<iostream>
using namespace std;

void temperature(int t)
{
    try{
        if (t==100) {
            throw "It's at the boiling point.";
        } else if (t==0) {
            throw "It reached the freezing point.";
        } else {
            cout << "the temperature = "<< t << endl;
        }
    }
    catch(int x) {
        cout << "temperature=" << x << endl;
    }
    catch(char const*s) {
        cout << s << endl;
    }
}

void handler(int n) throw(int, char, double) {
    if (n==1) throw n;
    if (n==2) throw 'x';
    if (n==3) throw 1.1;
}

int main() {
    cout << "1--befroe try block..." << endl;
    try {
        cout << "2--Inside try block..." << endl;
        throw 10;
        cout << "3--After throw ...." << endl;
    }
    catch(int i) {
        cout << "4--In catch block1 ... exception..errcode  is.." << i << endl;
    }
    catch(char * s) {
        cout << "5--In catch block2 ... exception..errcode is.." << s << endl;
    }
    cout << "6--After Catch...\n";

    temperature(0);
    temperature(10);
    temperature(100);

    try{
        handler(1);
    }
    catch(int i) { cout << "catch an integer..." << endl; }
    catch(char c) { cout << "catch an char..." << endl; }
    catch(double d) { cout << "catch an double..." << endl; }

    return 0;
}
