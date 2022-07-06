/*
在c++中，有三种正则可以选择使用，C ++regex，C regex，boost regex ，如果在windows下开发c++，默认不支持后面两种正则，如果想快速应用，显然C++ regex比较方便使用

C++ regex函数有3个：regex_match、regex_search、regex_replace
*/
#include <iostream>
#include <string>
#include <regex>
using namespace std;

int main()
{
	string s = "subject";
	regex e("(sub)(.*)");
	if(regex_match(s, e)) {
		cout << "string \""<< s <<"\" match regex\"(sub)(.*)\"" << endl;
	}

	if(regex_match(s.begin(), s.end(), e)) {
		cout << "range matched" << endl;
	}

	smatch sm;
	regex_match(s, sm, e);
	cout << "string object with " << sm.size() << " matches" << endl;

	// cbegin()和begin()之间的唯一区别是cbegin()返回指向常量值的常量迭代器
	regex_match(s.cbegin(), s.cend(), sm, e);
	cout << "range with " << sm.size() << " matches" << endl;

	cmatch cm;
	// using explicit flags:
	regex_match ("subject", cm, e, regex_constants::match_default);

	cout << "the matches were: ";
	for (unsigned i = 0; i < cm.size(); ++ i) {
		cout << "[" << cm[i] << "] ";
	}
	cout << endl;
	return 0;
}