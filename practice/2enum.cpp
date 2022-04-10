#include<iostream>
using namespace std;

enum weekday
{
 s,m,t,w,thu,f,s1
};

int main()
{
    enum weekday wek = s;
    // weekday wek=s;
    for(int i = wek; i != f; i ++)
    {
        cout << i << endl;
        cout << wek+s << endl;
        cout << "--------------" << endl;
    }
    return 0;
}

/* output:
0
0
--------------
1
0
--------------
2
0
--------------
3
0
--------------
4
0
--------------
*/
