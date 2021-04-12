#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	vector<int> a {2, 9, 5, 1, 4, 3};
	sort(a.begin(), a.end(), [](const int &x, const int &y) {
		return x < y;
	});
	for (int &i: a) {
		cout << i << ' ';
	}
	cout << endl;
	return 0;
}