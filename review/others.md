## Others

> 一些小厂面经整理

### 质谱华章

> https://www.zhipin.com/job_detail/b89ee9c54235a4e81XJ439u-FFNR.html
>
> 时间：2023.02.07
>
> 时长：30min

- 自我介绍 5 min

- 博客写的什么内容

- 视野预测项目遇到的难点

- 智力题：100! 有多少个0

  > 考虑 0 通过 5 * 2 = 10 产生，每个 5 都会产生一个 0，由于 2 和 5 中 2 的数目比较多，没那么重要，统计可以提供 5 的数量，100 / 5 = 20
  >
  > 另外 25 可以提供两个 5，100 / 25 = 4，所以最终可以产生 **20+4 = 24** 个零

> 技术栈不太匹配，二面不想约了



### 熠智科技

> 时间：2023.02.20
>
> 时长：60min

- 自我介绍 15min
- 项目 15min
  - 流式数据和静态数据的区别：视野预测模型如果针对一段一段数据的输出结果，如何处理，取均值？
  - 模型设计有什么特别点

#### 一面—C++

**1、一个类中默认的函数有哪些？**

6个默认成员函数：

- 构造函数

- 析构函数

- 拷贝构造函数：默认的拷贝构造函数对象按内存存储按字节序完成拷贝，是一种浅拷贝

  - 浅拷贝：也称位拷贝，编译器只是将对象中的值拷贝过来，就是简单的赋值。如果释放一个对象，访问另一个对象就会出现违规访问
  - 深拷贝：如果一个类中涉及到资源的管理，其**拷贝构造函数、赋值运算符重载以及析构函数必须要显式给出**，需要使用 new 构造新空间以及 strcpy 等拷贝变量

  > 参考：https://blog.csdn.net/Byersen/article/details/119704235

- 赋值运算符重载

  ```cpp
  void TestDate2()
  {
      Date d1(2022, 5, 18);
      Date d2(2022, 4, 18);
      Date d3 = d1;	// 等价于 Date d3(d1); 所以这里调用的是拷贝构造函数
      d2 = d1;		// 这里调用的是赋值构造函数
  }
  ```

- const 成员：注意 const 对象不能调用非 const 成员函数

  ```cpp
  // T::Print(T * const this) 参数类型为 T*, const 修饰的是 this
  void T::Print() { cout << "Print\n"; }
  
  // 实际上调用的是 t.Print(&t), &t 是 const T* 类型
  // const T* 传给 T* 是不对的，权限放大了，前者不可修改，后者可以 
  void func(const T& t) {
      t.Print();	// error
  }
  
  // const 对象只能调用 const 成员，所以改为
  // 实际上为 void T::Print(const T* const this)
  void T::Print_const() const { cout << "Print\n"; }
  
  // 权限可以不变或缩小
  void func(T& t) {
      t.Print();	// correct, 权限不变：T* 传给 T* 可以
      t.Print_const();	// correct, 权限缩小：T* 传给 const T* 可以
  }
  ```

- 取地址以及 const 取地址操作符重载：这两个运算符一般不需要重载，使用编译器生成的默认取地址的重载即可，只有特殊情况，才需要重载，比如想让别人获取到指定的内容

> 参考：https://blog.csdn.net/bit_zyx/article/details/124825804



**2、const 有什么作用，函数参数中使用这个干啥？**

```cpp
// 这两个函数有区别吗
void func(const string& str, char c)
void func(const string& str, const char c)
```



**3、指针常量和常量指针**

```cpp
// const 修饰是就近原则
const char* p; char const *p;	// 常量指针：指向常量的指针，const 修饰 *p
char * const p;	// 指针常量：指针本身是常量，const 修饰 p
```



**4、auto 关键字，类型推导的原则**

- auto 和 decltype 关键字实现类型推导，获取复杂类型，还可以简化书写
- auto定义的变量，可以根据初始化的值，在编译时推导出变量名的类型
- 可以同指针、引用结合起来使用，可以带上cv限定符 const 和 volatile

**推导规则**

（1）当不申明为指针或者引用时，auto的推导结果和初始化表达式抛弃引用和cv限定符后类型一致

（2）当声明为指针或引用时，auto的推导结果将保持初始化表达式的cv属性

> 参考：https://blog.csdn.net/weixin_58368590/article/details/124871252

**限制**

- auto不能用于函数参数
- auto不能用于非静态成员变量
- auto无法定义数组



**5、move 关键字，其中参数是什么，用来干什么的，里面具体实现细节**

move 可以用来实现转移语义，**但是 `std::move` 本身不会进行任何移动**，真正的移动操作是在移动构造函数、移动赋值函数等完成的，move 的作用就是(将左值)强制转换成右值引用（相当于一个 static_cast）

```cpp
// <utility> 头文件定义
// 以右值作为参数，但可以将左值强转为右值引用
template< class T >
typename std::remove_reference<T>::type&& move( T&& t ) noexcept;

// 可以自己实现
template <class _Ty>
constexpr std::remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept { // forward _Arg as movable
    return static_cast<std::remove_reference_t<_Ty>&&>(_Arg);
}
```

参考：

- [c++ move函数到底是什么意思？](https://www.zhihu.com/question/64205844)
- https://blog.csdn.net/qq_51568363/article/details/124285294



**6、万能引用是啥**

万能引用(universal reference)，也称为未定义引用，如果一个变量或参数被声明为 `T&&`，并且需要推导 `T` 的类型，那么该变量或参数即为万能引用。

万能引用，既可以是左值引用（绑定左值），也可以是右值引用（绑定右值）。

```cpp
#include <iostream>
using namespace std;

template <typename T>    // 函数模板
void myfunc(T&& val)     // 万能引用
{
    cout << val << endl;
    return;
}

int main()
{
    myfunc(120); // ok，自动类型推导，120是右值，T为int类型，val为int&&类型
    int i = 180;
    myfunc(i); // ok，自动类型推导，i是左值，T为int&类型，val为int&类型
    return 0;
}
```

注意：

- 区分右值引用和万能引用
- const 会剥夺万能引用的资格

> 参考：[[深入理解C++]万能引用、引用折叠](https://blog.csdn.net/qq_42815188/article/details/122751869)



#### 一面—操作系统

1、虚拟内存是干什么的

2、分页地址是什么形式，如果缺页如何替换

> 参考：[cyc2018——机操作系统 - 内存管理](http://www.cyc2018.xyz/计算机基础/操作系统基础/计算机操作系统%20-%20内存管理.html#虚拟内存)



#### HR 面

- 自我介绍 5min
- 实习时间点，为什么选择投该公司，之前还面过哪些公司
- 我们的业务点主要在于隐私保护，你之前主要关注流媒体



### Momenta

> https://www.zhipin.com/job_detail/476d83d1c40b8e801XF-3Ni6F1VS.html
>
> 时间：2023.02.08
>
> 时长：30min

**一面**

- 自我介绍 5min
- 视野预测的项目：怎样建模，多个用户请求怎么处理（多个模型）
- 非递归实现二叉树的前序遍历，[参考](https://github.com/EricPengShuai/Interview/blob/main/algorithm/%E4%BA%8C%E5%8F%89%E6%A0%91%E9%81%8D%E5%8E%86%E6%96%B9%E5%BC%8F.md)

> 反问：部门具体做啥（C++处理已经训练好的模型或者 feature map）



