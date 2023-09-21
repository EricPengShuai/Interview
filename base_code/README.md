## C++ base

> C++ 基础知识点

### C++ OOP

> 本科课程总结

#### 1. 概述

1. 类和对象关系是抽象和具体关系，对象是类的一个实例

2. OO基本特征：抽象、封装、继承、多态
   - 抽象：对具体问题进行概括，抽出这一类对象的公共性质并且加一描述，包括数据抽象和行为抽象
   - 封装：将抽象出的数据成员、行为成员相结合，将它们形成一个整体，访问权限的控制、部分成员充当外部接口
   - 继承：实现代码的重用，被继承的已有类称为基类或父类
   - 派生：在已有类的基础上新增自己的特性

3. 多态：同一名称，不同的功能实现方式，达到行为标识同一减少程序中标识符的个数
   
   - 函数重载：形参个数或者类型不同
   
   - 运算符重载：比如可以实现字符串相加
   
   - 虚函数：使用关键字`virtual`声明，在基类中申明一些能够在派生类中重新定义的函数

#### 2. 语言基础

1. 各种数据类型：字符型有单和宽之分，构造类型有结构类型`struct`、联合型`union`、枚举类型`enum`，比较重要的有**类类型**`class`

   - 前面可以加各种修饰符
   - 扩展类型如`struct`、`union`、`enum`可以被看成特殊的类

2. 常量`const`：在定义时或对象创建时，必须要进行初始化，之后在生命周期只读，不可以被修改，使用`define`定义常量不经过编译器进行类型检查，而`const`需要

3. 变量：不要随意定义全局变量或堆变量，尽量使用局部变量或栈变量。

   - 栈 stack：由编译器在需要的时候分配，在不需要的时候自动清除的变量的存储区，通常是局部变量、函数参数等。
   - 堆 heap：由`new`分配的内存块，他们的释放编译器不管，由应 用程序控制，一般一个`new`就要对应一个`delete`释放内存。
   - 自由存储区：由`malloc`等分配的内存块，和堆十分相似，不过是通过`free`来释放内存。
   - 静态存储区：全局变量和静态变量被分配在该存储区中。
   - 常量存储区：用于存放不能修改的常量，比如字符串常量 ，数字常量等。
   
4. 运算符和表达式：`&、 |、 ^、 ~`分别代表`与、或、异或、非`

5. 控制结构：顺序结构、分支结构、循环结构

6. ``inline``函数：在编译时调用内联函数的地方被函数体替换，减少函数调用所需的参数传递、控制转移等开销

   - 典型的空间换时间的手段

   - 内联函数定义必须出现在第一次被调用之前

7. 数组、指针变量、void类型指针的值（地址）可赋值给任何类型的指针变量但是需要强制类型转换

9. 指向常量的指针和和常量指针

#### 3. 类和对象

1. 三种成员类型
   - 公有成员：作为类与外部的接口
   
   - 私有成员：只有本类中的函数可以访问，派生类不可以访问
   
   - 保护类型：允许本类和派生类的成员访问
   
2. 构造函数：名字和类名相同，在创建对象时系统自动调动，可以有多个

3. 析构函数：在对象的生存期结束的时刻系统自动调用它， 然后再释放此对象所属的空间；如果未声明析构函数编译器自动产生一个默认的；基类析构函数不能被派生类继承

4. 拷贝构造函数：用类的一个对象初始化该类的另一个对象

5. `new`分配内存：`int *a = new int [5]`，`delete`销毁对象：`delete [] a`

6. 组合类构造函数和析构函数调用顺序

7. `this`指针隐含于类的成员函数中，指向正在被成员函数操作的对象

8. 友元函数：一种破坏数据封装和数据隐藏的机制

9. 静态数据成员

#### 4. 派生类和继承

1. 继承方式：默认时私有继承（全部变成私有），公有继承，保护继承
2. 派生类：不能继承基类的构造函数和析构函数，有单继承、多继承、直接基类、间接基类说法
3. 多继承基类会有二义性问题：类名限定或者同名隐藏



#### 优化OOP代码

注意不能返回局部的或者临时对象的指针或引用

1. ﻿函数参数传递过程中，**对象优先按引用传递，不要按值传递**
2. ﻿﻿函数返回对象的时候，应该**优先返回一个临时对象，而不要返回一个定义过的对象**
3. ﻿﻿接收返回值是对象的函数调用的时候，**优先按初始化的方式接收，不要按赋值的方式接收**

```cpp
class Test()
{
private:
    int a;
public:
    Test(int v=10) : a(v) {}
    Test(const Test&);
    void operator=(const Test &);
    ~Test() {}
    int getData() const { return a; }
};

Test getTest(Test &t) // 1. 按引用传递
{
    int val = t.getData();
    // Test tmp(val);
    return Test(val); // 2. 直接返回临时对象
}

int main()
{
    Test t1;
    // Test t2;
    // t2 = getTest(t1);
    Test t2 = getTest(t1); // 3. 优先按照初始化方式接受
}
```





### C++ Things

> 主要学习 https://github.com/Light-City/CPlusPlusThings

#### 1. const

- **作用**

  - 定义常量，防止变量修改，增加程序的健壮性
  - 节省空间，避免不必要的内存分配：const定义的常量在程序中只有一份拷贝，#define定义的常量在内存中有若干个拷贝

- const对象访问在不同文件访问时需要加extern关键字

- const常量定义时需要初始化，所以：`const i,j = 0;` 是不对的

- **指针**

  ```cpp
  const char * a;	// 指向const对象的指针或者是指向常量的指针
  char const * a;	// 同上
  char * const a;	// 指向字符对象的const指针，常指针
  const char * const a;	// 指向const字符对象的const指针
  ```

- 函数中使用const：[重点看一下](https://github.com/Light-City/CPlusPlusThings/tree/master/basic_content/const#6%E5%87%BD%E6%95%B0%E4%B8%AD%E4%BD%BF%E7%94%A8const)

- 类中使用const：[关注看一下](https://github.com/Light-City/CPlusPlusThings/tree/master/basic_content/const#7%E7%B1%BB%E4%B8%AD%E4%BD%BF%E7%94%A8const)
  - const对象只能访问const成员函数
  - 非const对象可以访问所有的成员函数



#### 2. static

- 静态变量：静态变量的空间只分配一次；类中的静态变量被实例化对象共享，所以类中的静态变量不能使用构造函数初始化

- 静态成员：类对象为静态时，一直到程序结束时才会被销毁；类中静态函数可以访问静态成员数据以及其他静态成员函数，无法访问类的非静态数据成员或成员函数，**注意一下访问方式**


> 代码参考：[static.cpp](static.cpp) && [static_in_class.cpp](static_in_class.cpp)



#### 3. this指针

- 作用：
  1. 一个对象的this指针不是对象本身的一部分
  2. this作用域在类内部，在类的非静态成员函数中访问类的非静态成员时，编译器会自动将对象本身的地址作为一个隐含参数传递给函数

- 使用：当参数与成员变量名相同时，使用`this->n = n`



#### 4. inline

**内联函数是以代码膨胀（复制）为代价提升函数执行效率的**

编译器处理内联函数步骤：

- 将`inline`函数体复制到`inline`函数调用点处
- 为所用`inline`函数中的局部变量分配内存空间
- 将`inline`函数的输入参数和返回值映射到调用方法的局部变量空间中
- 如果`inline`函数有多个返回点，将它转变为`inline`函数代码块末尾的分支

> 虚函数可以是内联函数，但是当虚函数表现多态性时不能内联（因为多态性是在运行期，内存是在编译期）
>
> 参考代码：[inline.cpp](inline.cpp)

- [ ] 修改文件名称



#### 5. sizeof

- 空类的大小为1字节
- 类中的虚函数、成员函数、静态数据成员都不占用类对象的存储空间，**即使类中有多个虚函数，但是只有一个虚指针**
- 虚函数继承都是继承基类的vptr

> 参考代码 ：[sizeof.cpp](sizeof.cpp)



#### 6. 纯虚函数和抽象类

##### 纯虚函数

- 只需要声明，不需要实现
- 包含纯虚函数的类就是抽象类，抽象类只能作为基类使用，**不能创建抽象类对象**，但是可以定义抽象类的指针

##### 抽象类

- **抽象类中成员函数可以调用纯虚函数，但是构造函数/析构函数内部不能使用纯虚函数**
- 如果一个类中集成抽象类，它只有实现实现了抽象类中所有的纯虚函数之后才能成为非抽象类

> 构造函数不能是虚函数，但是析构函数可以是虚函数



#### 7. vptr, vtable and virtual

##### vtable

每个使用虚函数的类（从虚函数派生出来的类）都有自己的虚拟表`vtable`。虚函数表包含可由类的对象调用的每个虚函数的一个条目，每个条目只是一个函数指针，指向该类可访问的派生函数

##### vptr

`vptr`是指向基类的指针，类创建实例时自动设置，和`this`指针不同，`this`指针实际上是编译器用来解析自引用的函数参数，vptr是一个真正的指针

##### virtual

:fire: 虚函数的调用取决于指向或者引用的对象的类型，而不是指针或者引用自身的类型

> 参考代码：[virtual.cpp](virtual.cpp)



**虚函数默认参数**

默认参数是静态绑定的，虚函数是动态绑定的。默认参数的使用需要看指针或者引用本身的类型，而不是对象的类型。

> 参考代码：[default_arg.cpp](default_arg.cpp)



##### 常见问题

:one: 静态函数可以声明为虚函数吗？

**静态函数不可以声明为虚函数，同时也不能被`const` 和 `volatile` 关键字修饰**

不可以，因为`static`成员函数不属于任何类对象或类实例，所以给此函数加上`virtual`没有任何意义



:two: 构造函数可以为虚函数吗？

构造函数不可以声明为虚函数，同时除了`inline/explicit`关键字之外，构造函数不允许使用其他任何关键字。

> 因为虚函数表`vtable`在编译阶段建立但是指向`vtable`的指针`vptr`在运行阶段实例化对象时才产生，如果类中有虚函数，编译器会在构造函数中添加代码来创建`vptr`，所以**如果构造函数是虚函数，那么他需要访问`vtable`，可这个时候`vptr`还没有产生**。



:three: 析构函数可以为虚函数吗？

析构函数可以为虚函数，**如果我们需要删除一个指向派生类的基类指针时，应该把析构函数声明为虚函数**。事实上，只要一个类有可能会被其他类所继承，就应该声明为虚析构函数。



:four: 虚函数可以为私有函数吗？

- 基类指针指向继承类对象，则调用继承类对象的函数
- int main() 必须声明为Base类的友元，否则编译失败。编译器报错：ptr无法访问私有函数。

> 代码参考：[virtual_function.cpp](virtual_function.cpp)



:five: 虚函数可以被内联吗？

通常成员数都会被编译器考虑是否进行内联。但通过基类指针或者引用调用的虚函数必定不能被内联。当然实体对象调用虚函数或者静态调用时可以被内联，虚析构函数的静态调用也一定会被内联展开。

- 虚函数可以是内联函数，内联是可以修饰虚函数的，但是当虚函数表现多态性的时候不能内联
- 内联是在编译器建议编译时内联，而虚函数的多态性是在运行期表现，编译器无法知道运行调用哪个代码，因此虚函数表现为多态性时不可以内联

> 代码学习： [virtual_inline.cpp](virtual_inline.cpp)



##### RTTI and dynamic_cast

- Run-Time Type Identification：通过运行时类型信息程序能够使用基类的指针或引用来检查这些指针或引用所指的对象的实际派生类型

- dynamic_cast 提供了类型安全检查，是一种基于能力查询的转换，所以在多态类型间进行转换更提倡采用 dynamic_cast

> 代码参考：[rtti.cpp](rtti.cpp)



#### 8. volatile

- `volatile`关键字是一种类型修饰符，用它声明的类型变量表示可以被某些编译器未知的因素（操作系统、硬件、其他线程）更改。所以使用`volatile`告诉编译器不应对这样的对象进行优化。
- `volatile` 关键字声明的变量，每次访问时都必须从**内存**中取出值（没有被`volatile`修饰的变量，可能由于编译器的优化，从**CPU寄存器**中取值）

:one: 一个参数既可以是const也可以是volatile吗？

可以。只读的状态寄存器可以是volatile，因为它可能被意想不到地改变，可以是const，因为程序不应该去修改它。

:two: 一个指针可以是volatile吗？

可以。当一个中断服务子程序修改一个指向buffer的指针时。

> 代码参考：[volatile.cpp](volatile.cpp)



#### 9. assert

是一个断言，是宏，不是函数，类似于Python中`assert`的用法

可以通过`NDEBUG`来关闭`assert`，但是需要在源代码的开头引入`assert`之前使用

> 参考代码：[assert.cpp](assert.cpp)



#### 10. extern

虽然C++兼容C，但是C++文件中函数编译后生成的符号与C语言生成的不同。因为C++虽然支持函数重载，C++函数编译之后生成的符号带有函数类型的信息，而C则没有。

> `int add(int a, int b)`函数经过C++编译器生成`.o`文件后，`add`会变成形如`add_int_int`，而C则会变成形如`_add`，所以相同的函数在C和C++中编译后生成的符号不同

如果C++中使用C语言实现的函数，我们使用`extern "C"`，在编译链接的时候就会告诉链接器去寻找C语言符号，而不是经过C++修饰的符号。



**C++调用C函数**

```cpp
//add.h
#ifndef ADD_H
#define ADD_H
int add(int x,int y);
#endif

//add.c
#include "add.h"

int add(int x,int y) {
    return x+y;
}

//add.cpp
#include <iostream>
using namespace std;
extern "C" {
    #include "add.h"
}
int main() {
    add(2,3);
    return 0;
}
```

编译时先通过gcc生成中间文件add.o：`gcc -c add.c`

然后编译：`g++ add.cpp add.o -o main`



**C调用C++函数**

:warning: `extern "C"`需要放在C++头文件中，放在C的头文件会报错

```cpp
// add.h
#ifndef ADD_H
#define ADD_H
extern "C" {
    int add(int x,int y);
}
#endif

// add.cpp
#include "add.h"

int add(int x,int y) {
    return x+y;
}

// add.c
extern int add(int x,int y);
int main() {
    add(2,3);
    return 0;
}
```

编译时先通过gcc生成中间文件add.o：`g++ -c add.cpp`

然后编译：`gcc add.c add.o -o main`

> 不过与C++调用C接口不同，C++确实是能够调用编译好的C函数，而这里C调用C++，不过是把C++代码当成C代码编译后调用而已。也就是说，C并不能直接调用C++库函数。



#### 11. struct

##### C中的struct

- C 中的struct只是单纯用作数据的符合类型，也就是说，在结构体声明中只能将数据成员放在里面，**不能将函数放在里面**
- C 的结构体不能继承
- 结构体的名字可以和函数名字相同

> 代码参考：[struct_func.c](struct_func.c)



##### C++中的struct

- C++结构体可以**定义函数**
- C++结构体定义时可以直接使用不用带struct，但是如果有同名函数需要加struct（C中不管是否有同名函数都必须要加struct）
- C++中的结构体可以继承

> 代码参考：[struct_function.cpp](struct_function.cpp)



#### 12. union

联合是一种节省空间的特殊的类，一个`union`可以有多个数据成员，但是在任意时刻只有一个数据成员可以有值。当某个成员被赋值后其他成员变为未定义状态。

- 默认访问控制符为`public`
- 可以含有构造函数和析构函数
- 不能含有引用类型的成员
- 不能继承其他类，不能做基类，不能含有虚函数
- 匿名`union`在定义所在作用域可直接访问`union`成员
- 全局匿名`union`必须是静态的

> 代码参考：[union.cpp](union.cpp)



#### 13. C 实现 C++ 面向对象思想

C++中的多态：在C++中会维护一张虚函数表

如果一个父类的指针或者引用调用父类的虚函数则该父类的指针会在自己的虚函数表中查找的自己的函数地址，如果该父类对象的指针或者引用指向的是子类的对象，而且该子类已经重写了父类的虚函数，则该指针会调用子类的已经重写的虚函数

> 参考代码：[c++_polymorphism.cpp](c++_polymorphism.cpp)



**C实现面向对象思想**

- 继承：使用函数指针把属性与方法封装到结构体重
- 多态：结构嵌套
- 类与子类方法的函数指针不同：在C语言的结构体内部是没有成员函数的，如何实现这个父类结构体和子结构体公有的函数呢？我们可以考虑使用函数指针来模拟。但是这样处理存在的一个缺陷就是：**父子各自的函数指针之间指向的不是类似C++中维护的虚函数表而是一块物理内存，如果模拟的函数过多就不容易维护**。

模拟多态必须保持函数指针变量对齐（**在内容上完全一致，而且变量对齐上也完全一致**）。否则父类指针指向子类对象，运行崩溃！

> 代码参考：[c_polymorphism.c](c_polymorphism.c)



#### 14. explicit

- `explicit`修饰构造函数时，可以防止**隐式转换**和**复制初始化**
- `explicit`修饰转换函数时，可以防止隐式转换，但按语境转换除外

>  代码参考：[explicit.cpp](explicit.cpp)



#### 15. friend

- 友元函数：可以访问某个类的私有或者保护成员，不是这个类的成员函数，可以在任何位置调用
- 友元类：类A中的成员函数可以访问另一个类B的私有或者保护成员，友元类A的声明在类B的声明中，而具体实现在类B外面

> 代码参考：[friend_func_class.cpp](friend_func_class.cpp)



- 友元关系没有继承性和传递性
- 友元虽然提高的程序的运行效率但是破坏了类的封装性和数据的透明性



#### 16. using

- 基本使用

  > 代码参考：[using_global.cpp](using_global.cpp)

- 改变访问性：访问父类的私有成员

  > 代码参考：[derived_base.cpp](derived_base.cpp)

- 函数重载

  > 代码参考：[using_derived.cpp](using_derived.cpp)

- 取代typedef

  ```cpp
  typedef vector<int> V1; 
  using V2 = vector<int>;
  ```



#### 17. `::`

- 全局作用域：`::name`
- 类作用域：`class::name`
- 命名空间作用域符：`namespace::name`

> 代码参考：[colon.cpp](colon.cpp)



#### 18. enum

- 枚举类

- 类中的枚举类型

> 枚举常量不会占用对象的存储空间，他们在编译时被全部求值
>
>  枚举常量的缺点：他的隐含数据类型是整数，其最大值有限，且不能表示浮点
>
> 代码参考：[enum.cpp](enum.cpp)



#### 19. decltype

- 推导出表达式类型

  ```cpp
  int i = 4;
  decltype(i) a;	// 推导结果为int。a的类型为int
  ```

- 与using/typedef合用，用于定义类型

  ```cpp
  using nullptr_t = decltype(nullptr);
  vector<int> vec;
  typedef decltype(vec.begin()) vectype;
  for(vectype i = vec.begin(); i != vec.end(); ++ i) { }	// 类似于auto
  ```

- 重用匿名类型

> 代码参考：[decltype.cpp](decltype.cpp)



#### 20. 指针和引用

- 引用必须初始化，指针可以不初始化

- 引用不能为空，指针可以为空：所以在使用指针是一般都需要判定是否为空

- 引用不能更换目标，指针可以

  ```cpp
  int a = 1;
  int b = 2;
  
  int &r = a;    //初始化引用r指向变量a
  int *p = &a;   //初始化指针p指向变量a
  
  p = &b;        //指针p指向了变量b
  r = b;         //引用r依然指向a，但a的值变成了b
  ```


- 右值引用：实现转移语义和精确传递
  - 消除两个对象交互时不必要的对象拷贝，节省运算存储资源，提高效率
  - 能够更简洁明确地定义泛型函数

> C++中引入了引用操作，在对引用的使用加了更多限制条件的情况下，保证了引用使用的安全性和便捷性，还可以保持代码的优雅性。在适合的情况使用适合的操作，引用的使用可以一定程度避免“指针满天飞”的情况，对于提升程序稳定性也有一定的积极意义。最后，**指针与引用底层实现都是一样的**，不用担心两者的性能差距。



#### 21. 宏

**字符串化操作符（#）**：在一个宏中的参数前面使用一个`#`，预处理器会把这个参数转换为一个字符数组，出现在宏定义中的`#`是把跟在后面的参数转换成一个字符串

> - 忽略传入参数名前面和后面的空格
>
> - 当有多个参数传入之间存在空格时，只会当成1个

**符号连接操作符（##）**：分隔连接方式，他的作用是先分隔，然后进行强制连接，将宏定义的多个形参转换成一个实际参数名。



**续行操作符（\）**：当定义的宏不能用一行表达完整时，可以用"\\"表示下一行继续此宏的定义

> 代码参考：[macro.cpp](macro.cpp)



### C++ MyThings

> 看施磊老师视频课程总结

#### template

模板实例化发生在调用模板函数时。当编译器遇到程序中对函数模板的调用时，它才会根据调用语句中实参的具体类型，确定模板参数的数据类型，并用此类型替换函数模板中的模板参数，生成能够处理该类型的函数代码，即模板函数。

当多次发生类型相同的参数调用时，只在第1次进行实例化。编译器只在第1次调用时生成模板函数，当之后遇到相同类型的参数调用时，不再生成其他模板函数，它将调用第1次实例化生成的模板函数。

函数模板实例化之后就是模板函数，类模板实例化之后就是模板类。

- 模板的实参推演：不用实例化直接推导参数类型，例如`compare(10, 20)` ，其中 compare 是函数模板

- 模板在调用点处被编译器替换成对应类型的模板函数/模板类

- 模板的特例化是用户提供的实例化模板函数

- 函数模板、模板特例化、非模板函数可以共存，编译器会怎么简单怎么来

参考：[template.cpp](template.cpp)



#### 重载

记住常见重载的写法，参考 [overload.cpp](overload.cpp)

malloc/free 和 new/delete 的区别？

- malloc 按字节开辟内存，new 开辟内存时需要指定类型，malloc 开辟内存返回的都是 void\*
- malloc 只负责开辟空间，new 不仅仅有的 malloc 的功能，还可以进行数据的初始化
- malloc 开辟内存失败返回 nullptr 指针；new 抛出的是 bad_alloc 类型的异常
- new/delete 是运算符，也可以被重载；malloc/free 是库函数，可以被覆盖

new 先底层调用 operator new（实际就是 malloc）申请内存, 然后再调用构造函数；delete 先调用析构函数，然后底层调用 operator delete （实际就是 free）释放内存，参考 [new_delete.cpp](new_delete.cpp)



#### 类型转换

把继承结构，也说成从上 （基类）到下（派生类） 的结构

```cpp
Base b;
Derive d;

// 基类对象 <-- 派生类对象 类型从下到上转换 Y
b = d;

// 派生类对象 <-- 基类对象 类型从上到下转换 N
// d = b;

// 基类指针（引用）<-- 派生类对象 类型从下到上的转换 Y
Base *pb = &d;

// 派生类指针（引用）--> 基类对象 类型从下到上的转换 N
Derive *pd = (Derive *)&b; // 不安全涉及了内存的非法访问
```

总结：在继承结构中进行上下的类型转换，默认只支持从下到上的类型的转换



#### 虛函数

1. 一个类里面定义了虚函数，那么编译阶段，编译器给这个类类型产生一个唯一的 vftable 虚函数表，虚函数表中主要存储的内容就是 RTTI 指针 (run-time type information，简单理解成类型字符串) 和虚函数的地址。当程序运行时，每一张虚函数表都会加载到内存的 **.rodata 区（只读区）**

2. 一个类里面定义了虚函数，那么这个类定义的对象在运行时，内存中开始部分，多存储一个 vfptr 虚函数指针，指向相应类型的虚函数表 vftable。这个类定义的多个对象的 vfptr 指向的都是**同一张虚函数表**

3. 一个类里面虚函数的个数，不影响对象内存大小 (因为只有一个 vfptr)，影响的是**虚函数表的大小**
4. 如果派生类中的方法，和基类继承来的某个方法，返回值、函数名、参数列表都相同，而且基类的方法是 virtual 虚函数，那么派生类的这个方法，自动处理成虚函数，也就是**重写/覆盖**



#### 多态

静态多态：编译时期多态，例如重载、模板

动态多态：运行时期多态，例如虚函数重写，基类指针（引用）指向派生类对象，通过指针（引用）调用同名的覆盖方法（虚函数），基类指针指向哪个派生类对象就会调用派生类对象的同名覆盖方法，**因为多态底层是通过动态绑定实现的**，通过基类指针可以访问派生类对象的 vptr，进而访问 vftable，最后访问到对应的虚函数

**并不是所有的虚函数的调用都是动态绑定**

- 在构造函数中都是静态绑定的，对象还没有初始化没有 vptr 
- 使用对象本身调用虚函数是静态绑定，**虚函数通过指针或者引用变量调用时才会发生动态绑定**

动态多态很好的体现了「开闭原则」，即对修改关闭、对扩展开放，例如有一个 API 使用 Animal  *p 作为参数，可以接收 &pig, &dog, &cat 等不同类型的动物，很好的做到了**高内聚低耦合**



#### 虚继承

- 抽象类：vfptr --> vftable
- 虚基类：vbptr --> vbtable，vbptr 在上，vfptr 在下，了解内存布局，代码参考：[virtual_base.cpp](virtual_base.cpp)

```cpp
A *p = new B(); // 基类指针指向派生类对象，永远指向的都是派生类基类部分数据的起始地址
cout << "A *p " << p << endl;
p->show();
delete p;
```

多重继承好处就是可以复用更多的代码，但是会有菱形继承的问题，也就是间接父类的成员会有多份出现在最终子类中，通过虚继承可以解决菱形继承问题，代码参考：[virtual_public.cpp](virtual_public.cpp)

> 建议再看看施磊老师这部分课程，内存分析很透彻


