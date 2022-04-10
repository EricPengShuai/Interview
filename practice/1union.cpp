#include <iostream>
using namespace std;

union myun
{
	struct 
	{
		int x;
		int y;
		int z;
	} u;
	int k;
}a;

int main()
{
	a.u.x = 4;
	a.u.y = 5;
	a.u.z = 6;
	a.k = 3;	// 覆盖掉第一个 int 的空间值
	cout << a.u.x << ' ' << a.u.y << ' ' << a.u.z << endl;
	cout << a.k << endl;
	return 0;
}

/* output:
3 5 6
3
*/