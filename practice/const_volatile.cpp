#include <stdio.h>
int main()
{
	// 可以通过volatile关键字修改const修饰的变量，但是不建议这么做
	// 参考：https://blog.csdn.net/weixin_41413441/article/details/80860135
	/* 
	1. C++ const 修饰符，表示常量，即如果以后保证不会修改则声明为const，否则若要修改，那一开始为什么还要声明为const呢？
	
	2. 根据C++标准，对于修改const变量，属于：未定义行为（指行为不可预测的计算机代码），这样一来此行为取决于各种编译器的具体实现（即不同编译器可能表现不同）。

	3. 故结论就是：不建议这么做！
	*/
	const volatile int i = 10;
	int* pi = (int*)(&i);
	*pi = 100;
	printf( "*pi: %d\n",*pi);
	printf( "i: %d\n",i);
	printf( "pi: %p\n",pi);
	printf( "&i: %p\n", &i);
	return 0;
}