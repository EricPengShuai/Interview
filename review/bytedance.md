## 广告部门

> 后端主要C++，少量Python/Golang

### 2020.11.20  一面 

#### C++基础问题

1. const是啥，[指针常量和常量指针](https://www.jb51.net/article/86519.htm)，怎样改变函数const声明的变量值

> ```c++
> // 首先应该明白指针就是地址 
> // p是一个常量指针【指向常量的指针】，指向的内容不可以变化，但是可以通过改变地址让它指向另一个常量	
> const int *p; 或者是 int const *p; 
> // p是一个指针常量【指针本身是常量】，指向内部存储的内存地址是常量，不可以改变，但是内存地址所对应的内容是可以变化的
> int *const p;	
> const int *const p; // 指向常量的常量指针
> ```
>
> - void function(const int Var);   //传递过来的参数在函数内不可以改变(无意义，该函数以传值的方式调用)
> - void function(const char* Var);  //参数指针所指内容为常量不可变
> - void function(char* const Var);  //参数指针本身为常量不可变(也无意义，var本身也是通过传值的形式赋值的)
> - void function(const Class& Var); //引用参数在函数内不可以改变
>
> volatile关键字：http://www.cppblog.com/mzty/archive/2006/08/08/10959.html
>
> ```c++
> // 修改construction修饰的变量
> // https://blog.csdn.net/weixin_41413441/article/details/80860135
> const volatile int i = 10；
> 
> // https://www.cnblogs.com/gylhaut/p/5502583.html
> # include <iostream>
> using namespace std;
> class TestMutable
> {
> public:
>   TestMutable(){i=0;}
>   int Output() const
>   {
>     return i++; 
>   }
> private:
> 	mutable int i;
> };
> ```



2. static是啥，比较，用在那些场景

> https://www.jianshu.com/p/0b2d9679a9f2
>
> - **限制符号的作用域只在本程序文件**
> - **指定变量的存储位置**: 全局的和函数内定义的**static**变量都是存放在数据区的，且只存一份，只在整个程序结束后才自动释放，其他变量都是存储在栈，函数结束之后就释放
> - **C++类的静态成员变量和函数**：静态成员函数中没有**this**指针，只能调用静态成员变量



3. 遍历一个vector所有元素的方法有哪些

> ```c++
> vector<int> a;
> // for循坏
> for(int i = 0; i < a.size(); i++) { 
> cout << a[i];
> 	cout << a.at(i);
> }
> 
> // 迭代器iterator  cbegin()  cend()
> // auto也可以换成vector<int>::const_iterator
> for (auto iter = valList.begin(); iter != valList.end(); iter++)
> {
> cout << (*iter) << endl;
> }
> 
> // for_each  cbegin()  cend()
> template<typename T>
> void printer(const T& val)
> {
> 	cout << val << endl;
> }
> void ShowVec(const vector<int>& valList)
> {
> 	for_each(valList.begin(), valList.end(), printer<int>);
> }
> 
> // 直接for auto遍历
> for (auto& val : valList)
> {
>  cout << val << endl;
> }
> ```



- `++i` 和 `i++`区别，哪个更快，为什么

> i++ 不能作为左值，而++i 可以。
>
> 1. a++是先用临时对象保存原来的对象，然后对原对象自增，再返回临时对象，不能作为左值；++a是直接对于原对象进行自增，然后返回原对象的引用，可以作为左值。
> 2. 由于要生成临时对象，a++需要调用两次拷贝构造函数与析构函数（将原对象赋给临时对象一次，临时对象以值传递方式返回一次）；++a由于不用生成临时变量，且以引用方式返回，故没有构造与析构的开销，效率更高。
>
> ```c++
> int i = 0;
> int *p1 = &(++i); //正确
> int *p2 = &(i++); //错误
> 
> ++i = 1; //正确
> i++ = 5; //错误
> 
> cout << ++(++(++i)) << endl;
> cout << ++ ++i << endl;
> ```
>
> 左值是对应内存中有确定存储地址的对象的表达式的值，而右值是所有不是左值的表达式的值。
>
> 左值与右值的根本区别在于是否允许取地址&运算符获得对应的内存地址。
>
> http://haoqchen.site/2018/10/15/difference-between-++i-i++-i+=1-i=i+1/



#### 计网和OS

1. 请求一个网页具体过程

   > 本地DNS服务器之前是查找浏览器、操作系统和路由器的DNS缓存，然后是参考[一次完整的HTTP请求过程图](https://github.com/EricPengShuai/Interview/blob/main/interview_summary.md#一次完整的http请求过程)

2. 进程通信方式

   > socket忘记了

3. 进程和线程区别

   > 差点把进程是资源分配的单位、线程是调度的基本单位说反了



**附带一点linux**

1. linux中vi模式的查找命令

   > 正则式：命令模式下输入“/字符串”，例如“/hive.sit”
   >
   > 如果查找下一个，按“n”即可



#### 算法

1. 有环链表的入口节点

   > 双指针，相遇之后，头再起一个指针知道和p1相遇

2. [1, 3, 1, 6]四个数字全部都用，使用加减乘除运算怎么得到24

   > 3 \* 8 和 4 \* 6



#### 反思

很多C++基础问题都不清楚，基础不牢



## 火山引擎

### 2021.4.23 一面

#### 操作系统

1. 进程调度的几个方法，说一下多级反馈队列调度
2. 死锁的条件：①互斥；②非抢占资源分配；③持有和等待；④循环等待
3. 进程和线程的区别
4. 虚拟内存了解吗
5. - [ ] 段页式了解吗



#### 数据库

1. 索引种类，底层实现是什么
2. - [ ] **B+树**的各种操作：插入（什么时候分裂），删除（什么时候合并），查找
3. - [ ] 行锁和表锁了解吗



#### 计算机网络

1. 网页请求过程，三次握手和**四次挥手**（挥手过程FIN字段是客户端先发一次然后服务器那边再发送一次，**CLOSE_WAIT**阶段，**TIME_WAIT**阶段）
2. - [ ] **get和post**携带的body区别
3. - [ ] 客户端请求的HTTP字段格式



#### 算法

1. 旋转数组的二分查找问题：重复和不重复



#### 反思

前面太紧张了，四次挥手过程，段页式也都忘了，没有问语言的知识啊