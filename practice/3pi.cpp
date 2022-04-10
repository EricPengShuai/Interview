#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;
/*
1. https://blog.csdn.net/qq_45782378/article/details/112985013 
*/

int main()
{
	int s = 1;
	double n = 1, t = 1, pi = 0;
	while (fabs(t) > 1e-7)
	{
		pi = pi + t;
		n += 2;
		s = -s;
		t = s / n;
	}
	pi = pi * 4;
	cout << "Π=" << setiosflags(ios::fixed) << setprecision(6) << pi << endl;
	return 0;
}

