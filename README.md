# Interview | 📚 by ericps

> 不定期添加和C++面试相关内容

![cpp_logo.png](https://s2.loli.net/2022/07/06/Acvw3IJBZFTnmb6.png)



## Guide Dir

> 不同公司的面试总结

- [ByteDanceGuide.md](./Guide/ByteDanceGuide.md)：字节面试指南
- [KuaishowGuide.md](./Guide/KuaishowGuide.md)：快手面试指南
- [Baidu.md](./Guide/baiduGuide.md)：百度面试指南



## Review dir

> 不同公司的面经

- [百度面经](./review/baidu.md)
- [字节面经](./review/bytedance.md)
- [快手面经](./review/kuaishou.md)
- [红海无线](./review/redInfinity.md)



## STL Dir

- C++一些**标准库**的常规使用
- 具体参考[README.md](./STL)



## algorithm dir

> 常见的LeetCode算法总结

- [背包问题](./algorithm/backpack.md)
- [二叉树迭代遍历方式](./algorithm/二叉树迭代遍历.md)
- [回溯算法](./algorithm/回溯算法.md)



## base_code dir

> C++基础关键字的用法，[C++_base.md](./C++_base.md) 中第二部分有相关描述

- [assert.cpp](./base_code/assert.cpp)：断言语句，只有在Debug版本才有效

- [c_polymorphism.c](c_polymorphism.c), [c++_polymorphism.cpp](./base_code/c++_polymorphism.cpp)：多态在c和c++中的实现

  > c中使用函数指针，`typedef void (*pf) ();  // 重定义一个函数指针的类型`
  >
  > c++中使用虚函数，`virtual override`

- [colon.cpp](./base_code/colon.cpp)：c++中冒号的使用，包括全局变量、类成员定义

- [decltype.cpp](./base_code/decltype.cpp)：推导表达式类型，与using/typedef联合使用定义类型

- ……



## practice dir

> C++实战代码和一些备忘知识点

- [unique_ptr.cpp](./practice/unique_ptr_ex.cpp)：智能指针
- [const_volatile.cpp](./practice/const_volatile.cpp)：通过**volatile**关键字修改**const**常量
- [static_in_class.cpp](./practice/static_in_class.cpp)：类中定义的静态成员变量不占类的大小，**在类外分配空间**；静态成员函数不能调用非静态成员，因为缺乏**this**指针
- [++i_i++.cpp](./practice/++i_i++.cpp)：**++i**和**i++**的[区别](https://www.php.cn/csharp-article-443596.html)，**++i**可以作为左值，`i++`不能作为左值（需要调用拷贝构造函数，测试之后只有一次？）
- [regexEx.cpp](./practice/regexEx.cpp)：C++正则表达式**regex**的用法
- [tieTuple.cpp](./practice/tieTuple.cpp)：C++标准库函数**tie**和**tuple**的用法
- [small_big_end.cpp](./practice/small_big_end.cpp): 大小端问题，“大端正着看，小端反着看”
- [accumulate.cpp](./practice/accumulate.cpp)：标准库的求和模板函数
- ......


## c++2.0 dir

> C++11/14/17/20新特性

- [lambda.cpp](./c++2.0/lambda.cpp)：lambda表达式基本使用
- [variadic.cpp](./c++2.0/variadic.cpp)：数量不定的模板参数

  

## memo dir

> 处理链表二叉树等数据结构以及输入处理技巧

- [listnode.cpp](./memo/listnode.cpp): 链表
- [treenode.cpp](./memo/treenode.cpp): 二叉树

  

## 其他

- [CMakeLists.txt](./CMakeLists.txt)：方便CLion同时编译多个文件的配置文件
- [C++\_base.md_](./C++_base.md)：来自北邮《C++面向对象程序设计实践》课程知识点和基础关键字代码

- [C_C++.md](./C_C++.md)：摘自[拓跋阿秀](https://github.com/forthespada/InterviewGuide)

