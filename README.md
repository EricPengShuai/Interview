# Interview | 📚 by ericps

> 不定期添加和C++面试相关内容

![cpp_logo.png](https://s2.loli.net/2022/07/06/Acvw3IJBZFTnmb6.png)



## 面试题总结

> 常见八股面试题总结，参考 `Guide` 文件夹

1. [Guide_1.md](Guide/Guide_1.md)
2. [Guide_2.md](Guide/Guide_2.md)
3. [Guide_3.md](Guide/Guide_3.md)
3. [Guide_4.md](Guide/Guide_4.md)



## C++ 基础

> C++ 基础关键字的用法，更多参考`base_code` 文件夹

1. [assert.cpp](base_code/assert.cpp)：断言语句，只有在Debug版本才有效
2. [c_polymorphism.c](c_polymorphism.c), [c++_polymorphism.cpp](base_code/c++_polymorphism.cpp)：多态在c和c++中的实现
   - C 中使用函数指针，`typedef void (*pf) ();` 表示重定义一个函数指针的类型
   - C++ 中使用虚函数，`virtual override`
3. [colon.cpp](base_code/colon.cpp)：C++ 中冒号的使用，包括全局变量、类成员定义
4. [decltype.cpp](base_code/decltype.cpp)：推导表达式类型，与 using/typedef 联合使用定义类型
4. [const_volatile.cpp](base_code/const_volatile.cpp): 通过`volatile`关键字修改`const`常量
4. [static_in_class.cpp](base_code/static_in_class.cpp): 类中定义的静态成员变量不占类的大小，**在类外分配空间**；静态成员函数不能调用非静态成员，因为缺乏`this`指针
5. 更多移步到 [base_code/readme](base_code) 



## C++ 新特性

> C++11/14/17/20新特性，`C++2.0` 文件夹

包括但不限于：
- 智能指针
- 引入了 auto 和 decltype 这两个关键字实现了类型推导
- 基于范围的 for 循环：for(auto& i : res){}
- 类和结构体的中初始化列表
- Lambda 表达式（匿名函数）
- std::forward_list（单向链表）
- 右值引用和move语义

具体参考 [C++2.0/README.md](C++2.0)



## 常见 STL 库

> C++ 一些**标准模板库**的常规使用，`STL` 文件夹

C++ Standard Library（标准库）包括 Standard Template Library（标准模板库），STL 分为六大类型：容器、分配器、算法、迭代器、适配器和仿函式

包括但不限于 vector, map, set, queue, queue_priority, sort 等等，具体参考 [STL/README.md](STL)



## 实践代码

> C++ 实战代码和一些备忘知识点，`practice` 文件夹

1. [unique_ptr.cpp](practice/unique_ptr_ex.cpp): 独占指针
4. [++i_i++.cpp](practice/++i_i++.cpp): `++i`和`i++`的[区别](https://www.php.cn/csharp-article-443596.html)，`++i`可以作为左值，`i++`不能作为左值（需要调用拷贝构造函数，测试之后只有一次？）
5. [regexEx.cpp](practice/regexEx.cpp): C++正则表达式`regex`的用法
6. [tieTuple.cpp](practice/tieTuple.cpp): C++标准库函数`tie`和`tuple`的用法
7. [small_big_end.cpp](practice/small_big_end.cpp): 大小端问题，“大端正着看，小端反着看”
8. [accumulate.cpp](practice/accumulate.cpp): 标准库的求和模板函数
8. [virtual_public.cpp](practice/virtual_public.cpp): 虚拟继承解决菱形继承问题
8. [itemPool.cpp](practice/itemPool.cpp): 重载 new/delete 实现**对象池** :fire:
8. [producer_consumer.cpp](producer_consumer.cpp): 互斥锁和条件变量实现生产者消费者模型 :fire:
9. 更多参考 [practice文件夹](practice) 



## 常见算法

> 常见 LeetCode 算法总结，`algorithm` 文件夹

|               :book:               |               :book:                |                     :book:                      |                      :book:                      |                :book:                |
| :--------------------------------: | :---------------------------------: | :---------------------------------------------: | :----------------------------------------------: | :----------------------------------: |
| [1.排序](algorithm/排序.md) :star: | [2.二分查找](algorithm/二分查找.md) |   [3.回溯算法](algorithm/回溯算法.md) :star:    |    [4.搜索算法](algorithm/搜索算法.md) :star:    | [5.差分数组](algorithm/差分数组.md)  |
|  [6.单调栈](algorithm/单调栈.md)   |   [7.并查集](algorithm/并查集.md)   | [8.二叉树遍历方式](algorithm/二叉树遍历方式.md) | [9.二叉树路径问题](algorithm/二叉树路径问题.md)  | [10.背包问题](algorithm/背包问题.md) |
|  [11.线段树](algorithm/线段树.md)  |  [12.字典树](algorithm/字典树.md)   |     [13.双指针](algorithm/双指针.md) :star:     | [14.最长上升子序列](algorithm/最长上升子序列.md) |   [15.数位DP](algorithm/数位DP.md)   |
| [16.其他算法](algorithm/others.md) |                                     |                                                 |                                                  |                                      |

**自定义常见数据结构**

> 处理链表二叉树等数据结构以及输入处理技巧，参考 `memo` 文件夹

1. [ListNode.cpp](memo/listnode.cpp): 链表
2. [TreeNode.cpp](memo/treenode.cpp): 二叉树



## Linux 相关
> 《Linux 高性能服务器编程》阅读笔记，`Linux` 文件夹

详见 [Linux-readme.md](Linux/readme.md)

- 参考1：《Linux 高性能服务器编程》游双著
- 参考2：牛客——[Linux高并发服务器开发](https://www.nowcoder.com/courses/cover/live/504)



## 数据库

- [MySQL](MySQL.md)
- [Redis](Redis.md)



## 设计模式

> 常见设计模式

- 单例模式：饿汉式、懒汉式
- 工厂模式：简单工厂、抽象工厂
- 观察者模式

参考：[DesignPattern.md](DesignPattern.md)



## 流媒体相关

> 流媒体相关知识点，常见协议说明，包括 RTMP、RTSP、HLS、DASH 等

参考：[Streaming.md](Streaming.md)



## 面经汇总

> 不同公司的面经，`review` 文件夹

|           :book:           |             :book:              |             :book:             |             :book:              |            :book:            |
| :------------------------: | :-----------------------------: | :----------------------------: | :-----------------------------: | :--------------------------: |
|  [百度](review/Baidu.md)   |    [阿里](review/Alibaba.md)    |   [腾讯](review/Tencent.md)    | [字节跳动](review/Bytedance.md) |  [快手](review/Kuaishou.md)  |
| [美团](review/Meituan.md)  |   [京东](review/Jingdong.md)    | [蚂蚁集团](review/Antgroup.md) |   [小红书](review/Redxhs.md)    |   [游戏厂](review/Game.md)   |
| [国企银行](review/Bank.md) | [国企子公司](review/SubBank.md) | [金融科技](review/Fintech.md)  |  [武汉企业](review/WuhanIT.md)  | [其他面经](review/Others.md) |




## 其他

- [CMakeLists.txt](CMakeLists.txt)：方便 CLion 同时编译多个文件的配置文件
- [C_C++.md](C_C++.md)：C/C++八股文，大部分摘自[拓跋阿秀](https://github.com/forthespada/InterviewGuide)，外加自己勘误
- [CPPhelper.md](CPPhelper.md)：一些关于 CPP 的零碎知识，例如 g++, CMakeLists.txt, makefile 等

