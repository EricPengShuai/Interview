# Interview | 📚 by ericps

> 不定期添加和C++面试相关内容

![cpp_logo.png](https://s2.loli.net/2022/07/06/Acvw3IJBZFTnmb6.png)



## Guide Dir

> 不同公司的面试总结

1. [ByteDanceGuide.md](./Guide/ByteDanceGuide.md)：字节面试指南

2. [KuaishouGuide.md](./Guide/KuaishouGuide.md)：快手面试指南

3. [Baidu.md](./Guide/baiduGuide.md)：百度面试指南



## Review dir

> 不同公司的面经

1. [百度面经](./review/baidu.md)

2. [字节面经](./review/bytedance.md)

3. [快手面经](./review/kuaishou.md)

4. [红海无线](./review/redInfinity.md)

5. [其他面经](./review/others.md)



## STL Dir

- C++一些**标准库**的常规使用
- 具体参考 [STL/README.md](./STL)



## algorithm dir

> 常见的LeetCode算法总结

1. [背包问题](./algorithm/背包问题.md)
2. [二分查找](./algorithm/二分查找.md)
3. [回溯算法](./algorithm/回溯算法.md)
4. [搜索算法](./algorithm/搜索算法.md)
5. [差分数组](./algorithm/差分数组.md)
6. [单调栈](./algorithm/单调栈.md)
7. [并查集](./algorithm/并查集.md)
8. [二叉树遍历方式](./algorithm/二叉树遍历方式.md)
9. [二叉树路径问题](./algorithm/二叉树路径问题.md)
10. [排序](./algorithm/排序.md)
11. [线段树](./algorithm/线段树.md)
12. [字典树](./algorithm/字典树.md)
13. [双指针](./algorithm/双指针.md)
14. [最长上升子序列](./algorithm/最长上升子序列.md)
15. [数位DP](./algorithm/数位DP.md)
16. [其他算法](./algorithm/others.md)



## base_code dir

> C++基础关键字的用法，[C++_base.md](./C++_base.md) 中第二部分有相关描述

1. [assert.cpp](./base_code/assert.cpp)：断言语句，只有在Debug版本才有效

2. [c_polymorphism.c](c_polymorphism.c), [c++_polymorphism.cpp](./base_code/c++_polymorphism.cpp)：多态在c和c++中的实现

   > c中使用函数指针，`typedef void (*pf) ();  // 重定义一个函数指针的类型`
   >
   > c++中使用虚函数，`virtual override`

3. [colon.cpp](./base_code/colon.cpp)：c++中冒号的使用，包括全局变量、类成员定义
4. [decltype.cpp](./base_code/decltype.cpp)：推导表达式类型，与using/typedef联合使用定义类型
5. 更多参考 [base_code文件夹](./base_code) ...



## practice dir

> C++实战代码和一些备忘知识点

1. [unique_ptr.cpp](./practice/unique_ptr_ex.cpp)：智能指针
2. [const_volatile.cpp](./practice/const_volatile.cpp)：通过**volatile**关键字修改**const**常量
3. [static_in_class.cpp](./practice/static_in_class.cpp)：类中定义的静态成员变量不占类的大小，**在类外分配空间**；静态成员函数不能调用非静态成员，因为缺乏**this**指针
4. [++i_i++.cpp](./practice/++i_i++.cpp)：**++i**和**i++**的[区别](https://www.php.cn/csharp-article-443596.html)，**++i**可以作为左值，`i++`不能作为左值（需要调用拷贝构造函数，测试之后只有一次？）
5. [regexEx.cpp](./practice/regexEx.cpp)：C++正则表达式**regex**的用法
6. [tieTuple.cpp](./practice/tieTuple.cpp)：C++标准库函数**tie**和**tuple**的用法
7. [small_big_end.cpp](./practice/small_big_end.cpp): 大小端问题，“大端正着看，小端反着看”
8. [accumulate.cpp](./practice/accumulate.cpp)：标准库的求和模板函数
9. 更多参考 [practice文件夹](./practice) ...



## c++2.0 dir

- C++11/14/17/20新特性
- 具体参考 [c++2.0/README.md](./c++2.0)

  

## memo dir

> 处理链表二叉树等数据结构以及输入处理技巧

1. [listnode.cpp](./memo/listnode.cpp): 链表
2. [treenode.cpp](./memo/treenode.cpp): 二叉树



## 其他

- [CMakeLists.txt](./CMakeLists.txt)：方便CLion同时编译多个文件的配置文件
- [C_C++.md](./C_C++.md)：C/C++八股文，摘自[拓跋阿秀](https://github.com/forthespada/InterviewGuide)
- [CPPhelper.md](./CPPhelper.md)：一些关于 CPP 的零碎知识，例如 g++, CMakeLists.txt, makefile 等
- [C++\_base.md](./C++_base.md)：来自北邮《C++面向对象程序设计实践》课程知识点和基础关键字代码

