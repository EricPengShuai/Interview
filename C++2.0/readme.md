# C++ 2.0

## C++11&14

> 参考 《C++ 新标准 C++11&14》——侯捷
>
> 笔记参考：https://blog.csdn.net/qq_15041569/article/details/110755182

### 语言部分

- **varadic template** 可变类型参数模板，initializer_list 接受相同类型的可变参数，template 主要指的是函数模板和类模板

  ```cpp
  void print() { // 递归出口
  }
   
  template <typename T, typename... Types>                
  //这里的...是关键字的一部分：模板参数包
  void print(const T& firstArg, const Types&... args)     
  //这里的...要写在自定义类型Types后面：函数参数类型包
  {
      cout << firstArg << endl;
      print(args...); //这里的...要写在变量args后面：函数参数包
  }
  ```

- **default/delete** 作用：为了增强对“类默认函数的控制”，从而让程序员更加精准地去控制默认版本的函数

  - 六大默认函数：构造、析构、拷贝构造、拷贝赋值、移动构造、移动赋值，default 和 delete 基本就作用在这5种函数上（析构除外）
  - 何时需要自定义 big-three（构造函数、拷贝构造、拷贝赋值），如果类中带有point member（指针成员），那我们就可以断定必须要给出 big-three

- **using** 用来给类型取别名，且这个别名化是可以带参数的，define 和 typedef 无法达到同样的效果，template template parameter + alias template

  ```cpp
  template <typename T,
            template <typename T>
                class Container
           >
  class XCls
  {
  private:
      Container<T> c;
  public:
      XCLs()
      {
          for(long i=0; i<SIZE; ++i)
              c.insert(c.end(), T());
   
          output_static_data(T());
          Container<T> c1(c);
          Container<T> c2(std::move(c));
          c1.swap(c2);
      }
  };
  
  XCls<MyString, vector> c1; // error, vector 第二个默认参数需要指定
  
  template <typename T>
  using Vec = vector<T, allocator<T>>;
  XCls<MyString, Vec> c1;    // ok
  ```

- **noexcept** 用于申明函数保证不会抛出异常，后面可以跟一个括号写一个条件，也就是说在某种条件满足情况下，不会抛出异常

- **override** 用于明确要重写父类的虚函数上，相当于告诉编译器这个函数就是要重写父类虚函数这样一个意图，让编译器帮忙检查，而没有这个关键字，编译器是不会帮你检查的

- **final** 禁止基类被继承，禁止虚函数被重写

- **decltype** 用来声明函数的返回值类型、模板之间的应用、用于求 lambda 表达式的类型

  ```cpp
  template<typename T1, typename T2>
  auto Add(T1 x, T2 y) -> decltype(x+y);
  ```

- **lambda** 表达式，可以大大简化原始仿函数的写法

  ```cpp
  // mutable throwSpec retType 是可选的
  // mutable 表示 [] 中的导入数据是否可变
  // throwSpec 表示抛出异常
  // [...](...) mutable throwSpec -> retType {...}
  
  int id = 0;
  auto f = [id]() mutable {
      cout << id << endl;
      ++ id; // 有 mutable 才能在值传递时 ++ id
  };
  id = 42;
  f(); f(); f(); // 0 1 2 (id 是值传递)
  cout << id << endl; // 42
  ```

  

### 标准库

- **临时对象就是一个右值**，右值不要出现在等号左边（虽然 string 和 complex 可以写）

- 使用 std::move() 可以得到一个左值的右值引用，移动语言就是一个「偷」的过程，为了让「偷」的过程不被中间商影响，引入了完美转发 std::forward()

  ```cpp
  // 带有移动语义的类
  class MyString{
  public:
      static size_t DCtor;        //累计default-ctor呼叫次数
      static size_t Ctor;         //累计ctor呼叫次数
      static size_t CCtor;        //累计copy-ctor呼叫次数
      static size_t CAsgn;        //累计copy-asgn呼叫次数
      static size_t MCtor;        //累计move-ctor呼叫次数
      static size_t MAsgn;        //累计move-asgn呼叫次数
      static size_t Dtor;         //累计dtor呼叫次数
  private:
      char* _data;
      size_t _len;
      void _init_data(const char *s){
          _data = new char[_len + 1];
          memcpy(_data, s, _len);
          _data[_len] = '\0';
      }
  public:
      //default constructor
      MyString() : _data(NULL), _len(0) { ++DCtor; }
   
      //constructor
      MyString(const char* p) : _len(strlen(p)){
          ++Ctor;
          _init_data(p);
      }
   
      //copy constructor
      MyString(const MyString& str) : _len(str._len){
          ++CCtor;
          _init_data(str._data);
      }
   
      // move constructor, with "noexcept"
      MyString(MyString&& str) noexcept
          : _data(str._data), _len(str._len){
              ++MCtor;
              str._len = 0;
              str._data = NULL; // 重要，析构的时候不要 delete 掉
      }
   
      // copy assignment
      MyString& operator=(const MyString& str){
          ++ CAsgn;
          if(this != &str){
              if(_data) delete _data;
              _len = str._len;
              _init_data(str._data);
          }
          else{
          }
          return *this;
      }
   
      //move assignment
      MyString& operator=(MyString&& str) noexcept{
          ++ MAsgn;
          if(this != &str){
              if(_data) delete _data;
              _len = str._len;
              _data = str._data;
              str._len = 0;
              str._data = NULL;
          }
          return *this;
      }
   
      //dtor
      virtual ~MyString(){
          ++Dtor;
          if(_data){
              delete _data;
          }
      }
   
      bool operator<(const MyString& rhs) const
      {
          return string(this->_data) < string(rhs._data);
      }
   
      bool operator==(const MyString& rhs) const
      {
          return string(this->_data) == string(rhs._data);
      }
   
      char* get() const { return _data; }
  };
  size_t MyString::DCtor = 0;
  size_t MyString::Ctor = 0;
  size_t MyString::CCtor = 0;
  size_t MyString::CAsgn = 0;
  size_t MyString::MCtor = 0;
  size_t MyString::MAsgn = 0;
  size_t MyString::Dtor = 0;
   
  namespace std    //必须放在std内
  {
  template<>
  struct hash<MyString>{    //for unordered containers
      size_t operator()(const MyString& s) const noexcept
      { return hash<string>() (string(s.get())); } 
  }；
  ```



## modern-cpp

> 参考 https://github.com/changkun/modern-cpp-tutorial

  

### 1. 迈向现代 C++

#### 1.1 被弃用的特性

- 善用 const char * 和 auto

```cpp
// 弃用警告，建议使用 const char* 或者 auto
char *str = "hello world!";
```

- C语言风格的类型转换被弃用，建议使用 static_cast, reinterpret_cast, const_cast 来进行类型转换
- auto_ptr 被弃用，应该使用 unique_ptr

  

#### 1.2 与 C 的兼容性

在编写 C++ 时尽量避免使用 C 文件，如非不可应该注意使用 extern "C" 这种特性，将 C 语言的代码与 C++ 代码进行分离编译，再统一链接

```bash
// 先将 C 代码编译成二进制文件
gcc -c foo.c
clang++ 1.1.cpp foo.o -o 1.1
```

  

### 2. 语言可行性的强化

#### 2.1 常量

- C++11 使用 nullptr 代替 NULL，传统 C++ 会将 NULL、0 视为同一种东西，取决于编译器。**尽量使用 nullptr**

- C++17 引入 constexpr 让用户显式的声明函数或者对象构造函数在编译期会成为**常量表达式**，因而可以强化 const 意思

  ```cpp
  int len = 1;
  const int len2 = len + 2;
  constexpr int len3 = len + 3;
  char arr[len2];		// 在某些老版本的编译器中会报错
  char arr_2[len3];	// 合法
  ```

    

#### 2.2 变量及其初始化

- if/switch 语句可以放在括号里面定义以及初始化

  ```cpp
  // vec 是 vector<int> 容器实例化对象
  if (const auto itr = find(vec.begin(), vec.end(), 2); itr != vec.end()) {
      *itr = 3;
  }
  ```

- 结构化绑定：C++17引入该思想可以使用 auto 直接推断出 tuple 元组各个元素

  ```cpp
  std::tuple<int, double, std::string> f() {
      return std::make_tuple(1, 2.3, "456");
  }
  
  auto [x, y, z] = f();
  ```


##### initialize_list 

初始化列表：C++11 引入 initializer_list 允许构造函数或其他函数像像参数一样使用初始化列表，可以使用 {} 进行初始化（**类中构造函数初始化顺序和参数定义顺序相关**）

- initializer_list\<T> 背后有 array 数组支撑，initializer_list 它其实是关联一个 array\<T,n>
- 源码实现中只是对 array 指针的一个浅拷贝动作，比较危险
- max 和 min 可以接受任意参数，实际内部调用的是 *max_element 和 *min_element



#### 2.3 类型推导

- C++11 引入了 auto 和 decltype 这两个关键字实现了类型推导，让编译器来操心变量的类型；C++20 开始 auto 可以用于参数传递

  ```cpp
  auto i = 5;
  auto arr = new auto(10);	// arr 被推导为 int *
  
  auto add(auto x, auto y) {	// 可能报错，最好使用模块
      return x + y;
  }
  ```

- decltype 用于推断某个表达式的类型

  ```cpp
  auto x = 1;
  auto y = 2;
  decltype(x+y) z;	// z 就是整形
  ```

- C++11 还引入了一个叫做尾返回类型`->`（tailing return type），利用 auto 关键字将返回类型后置

  ```cpp
  template<typename T, typename U>
  auto add2(T x, U y) -> decltype(x+y){
  	return x + y;
  }
  
  auto w = add2<int, double>(1, 2.3);
  ```

    C++14 开始可以直接让普通函数具有返回值推导，因此可以将 decltype(x+y) 去掉

  

#### 2.4 控制流

- C++17 将 constexpr 这个关键字引入到 if 语句中，允许在代码中声明常量表达式的判断条件，这样让代码在编译时就完成分支判断，效率更高

  ```cpp
  template<typename T>
  auto print_type_info(const T& t) {
      if constexpr (std::is_integral<T>::value) {
      	return t + 1;
      } else {
      	return t + 0.001;
      }
  }
  // 编译之后实际上变成如下表达
  int print_type_info(const int& t) {
  	return t + 1;
  }
  double print_type_info(const double& t) {
  	return t + 0.001;
  }
  ```

- 区间 for 迭代：C++11 引入了基于范围的迭代写法，我们拥有了能够写出像 Python 一样简洁的循环语句

  ```cpp
  for (auto element : vec)
  	std::cout << element << std::endl; // read only
  for (auto &element : vec) 
  	element += 1; // writeable
  ```

  

#### 2.5 模板

- C++11 引入了**外部模板**，扩充了原来的强制编译器在特定位置实例化模板的语法，使我们能够显式的通知编译器何时进行模板的实例化，节省编译时长

- C++11 之后连续的尖括号 >> 变得合理

- **模板是用来产生类型的**。在传统的 C++ 中，typedef 可以为类型定义一个新的名称，但是没办法为模板定义一个新的名称。因为模板不是类型

  ```cpp
  template<typename T, typename U>
  class MagicType {
  public:
      T dark;
      U magic;
  };
  // 不合法
  template<typename T>
  typedef MagicType<std::vector<T>, std::string> FakeDarkMagic;
  ```

  C++11 使用 using 引入了下面这种形式，并且同时支持对传统 typedef 相同的功效

  ```cpp
  template<typename T>
  using TrueDarkMagic = MagicType<std::vector<T>, std::string>;
  ```

- **变长参数模板**：模板一直是 C++ 所独有的黑魔法（一起念： Dark Magic）之一。在 C++11 之前，无论是类模板还是函数模板，都只能按其指定的样子，接受一组固定数量的模板参数；而 C++11 加入了新的表示方法，**允许任意个数、任意类别的模板参数，同时也不需要在定义时将参数的个数固定**。  

  ```cpp
  template<typename... Ts> class Magic;
  
  template<typename... Ts>
  void magic(Ts... args) {
      std::cout << sizeof...(args) << std::endl;	// sizeof... 计算参数的个数
  }
  ```

  **解包1：递归模板函数**

  ```cpp
  #include <iostream>
  template<typename T0>
  void printf1(T0 value) {
      std::cout << value << std::endl;
  }
  template<typename T, typename... Ts>
  void printf1(T value, Ts... args) {
      std::cout << value << std::endl;
      printf1(args...);
  }
  int main() {
      printf1(1, 2, "123", 1.1);
      return 0;
  }
  ```

  **解包2：变参模板展开**

  ```cpp
  template<typename T0, typename... T>
  void printf2(T0 t0, T... t) {
      std::cout << t0 << std::endl;
      if constexpr (sizeof...(t) > 0) printf2(t...);
  }
  ```

- **折叠表达式**：**C++ 17** 中将变长参数这种特性进一步带给了表达式  

  ```cpp
  #include <iostream>
  template<typename ... T>
  auto sum(T ... t) {
      return (t + ...);
  }
  int main() {
      std::cout << sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << std::endl;
  }
  ```

- **非类型模板参数推导**：在 C++11 引入了类型推导这一特性后，我们会很自然的问，既然此处的模板参数以具体的字面量进行传递，能否让编译器辅助我们进行类型推导，通过使用占位符 auto 从而不再需要明确指明类型？幸运的是， C++17 引入了这一特性，我们的确可以 auto 关键字，让编译器辅助完成具体类型的推导

  ```cpp
  template <auto value> 
  void foo() {
  	std::cout << value << std::endl;
      return;
  }
  int main() {
  	foo<10>(); // value 被推导为 int 类型
  }
  ```
  
  

#### 2.6 面向对象

- **委托构造**：C++11 引入了委托构造的概念，这使得构造函数可以在同一个类中一个构造函数调用另一个构造函数

- **继承构造**：在传统 C++ 中，构造函数如果需要继承是需要将参数一一传递的，这将导致效率低下。 C++11 利用关键字 using 引入了继承构造函数的概念  （具体参考《modern-cpp-tutorial-zh.cpp》）

- **显式虚函数重载**：当使用 override 重载虚函数时，将显式的告知编译器进行重载，编译器将检查基函数是否存在这样的虚函数，否则将无法通过编译；当使用 final 时则是为了防止类被继续继承以及终止虚函数继续重载引入的  

- **显式禁用默认函数**

- **强类型枚举**：在传统 C++ 中，枚举类型并非类型安全，枚举类型会被视作整数，则会让两种完全不同的枚举类型可以进行直接的比较（虽然编译器给出了检查，但并非所有）， **甚至同一个命名空间中的不同枚举类型的枚举值名字不能相同**，这通常不是我们希望看到的结果  。

  C++11 引入了枚举类（enumeration class），并使用 enum class 的语法进行声明  

  ```cpp
  enum class new_enum : unsigned int {
      value1,
      value2,
      value3 = 100,
      value4 = 100
  };
  ```

  我们希望获得枚举值的值时，将必须显式的进行类型转换，不过我们可以通过重载 << 这个算符来
  进行输出

  ```cpp
  #include <iostream>
  
  template<typename T>
  std::ostream& operator<<(
      typename std::enable_if<std::is_enum<T>::value,
      std::ostream>::type& stream, const T& e)
  {
      return stream << static_cast<typename std::underlying_type<T>::type>(e);
  }
  ```

  

### 3. 语言运行期的强化

#### 3.1 lambda表达式

**基本语法**

```cpp
[捕获列表](参数列表) mutable(可选) 异常属性 -> 返回类型 {
// 函数体
}
```

- 值捕获：[var]
- 引用捕获：[&var]
- 隐式捕获：[], [&], [=]

- 表达式捕获：`[var = 1, var2 = std::move(std::make_unique<int>(1))]`



**泛型 Lambda**

C++14之前 auto 关键字不能够用在参数表里，这是因为这样的写法会与模板的功能产生冲突。但是 Lambda 表达式并不是普通函数，所以 Lambda 表达式并不能够模板化。这就为我们造成了一定程度上的麻烦：参数表不能够泛化，必须明确参数表类型  

从 C++14 开始， Lambda 函数的形式参数可以使用 auto 关键字来产生意义上的泛型  

```cpp
auto add = [](auto x, auto y) {
	return x+y;
};
```



#### 3.2 函数对象包装器 :fire:

- std::function

  ```cpp
  int foo(int para) {
  	return para;
  }
  // std::function 包装了一个返回值为 int, 参数为 int 的函数
  std::function<int(int)> func = foo;
  ```

- std::bind std::placeholder

  bind 是用来绑定函数调用的参数的，它解决的需求是我们有时候可能并不一定能够一次性获得调用某个函数的全部参数，通过这个函数，我们可以将部分调用参数提前绑定到函数身上成为一个新的对象，然后在参数齐全后，完成调用。例如：  
  
  ```cpp
  int foo(int a, int b, int c);
  int main() { 
    // 将参数 1,2 绑定到函数 foo 上，但是使用 std::placeholders::_1 来对第一个参数进行占位 
    auto bindFoo = std::bind(foo, std::placeholders::_1, 1,2); 
    // 这时调用 bindFoo 时，只需要提供第一个参数即可 
    bindFoo(1); 
  }
  ```
  

1. function 类模板定义出可以看出希望使用一个函数类型实例化
2. 通过 function 调用 operate() 函数时要传入相应的参数

```cpp
class Test
{
public: // 必须依赖一个对象 void (Test::*hello)(string)
    void hello(string str) { cout << str << endl; }
};

// 默认有一个 this 指针作为参数
function<void(Test*, string)> f = &Test:hello;
f(&Test(), "call Test::hello!");
```

原理：实际就是重载了 operator() 操作符，参考 [bind_function.cpp](bind_function.cpp)



#### 3.3 右值引用

- **左值** (lvalue, left value)，就是赋值符号左边的值。准确来说，左值是表达式（不一定是赋值表达式）后依然存在的持久对象。

- **右值** (rvalue, right value)，右边的值，是指表达式结束后就不再存在的临时对象。C++11 将右值划分为纯右值和将亡值。

  - **纯右值** (prvalue, pure rvalue)：要么是纯粹的字面量，例如 10, true；要么是求值 结果相当于字面量或匿名临时对象，例如 1+2。**非引用返回的临时变量、运算表达式产生的临时变量、原始字面量、Lambda 表达式都属于纯右值**。

    > 字符串字面量只有在类中才是右值，当其位于普通函数中是左值。

  - **将亡值** (xvalue, expiring value)，是 C++11 为了引入右值引用而提出的概念（因此在传统 C++ 中，纯右值和右值是同一个概念），也就是即将被销毁、却能够被移动的值。(具体参考pdf)

  

- 右值引用和左值引用

  C++11 提供了 std::move 这个方法将左值参数无条件的转换为右值

  ```cpp
  void reference(std::string& str) { 
      std::cout << " 左值" << std::endl; 
  } 
  
  void reference(std::string&& str) {
      std::cout << " 右值" << std::endl; 
  }
  
  string lv1 = "string,", lv2 = "string,"
  string&& rv2 = lv1;			// 非法, 右值引用不能引用左值
  string&& rv2 = lv1 + lv2; 	// 合法, 右值引用延长临时对象生命周期
  rv2 += "Test"; 				// 合法, 非常量引用能够修改临时变量
  cout << rv2 << endl; 		// string,string,string,Test
  
  // rv2 虽然引用了一个右值，但由于它是一个引用，所以 rv2 依然是一个左值
  reference(rv2); 	// 输出" 左值"
  ```

  

- 移动语义

  传统的 C++ 没有区分『移动』和『拷贝』的概念，造成了大量的数据拷贝，浪费时间和空间。右值 引用的出现恰好就解决了这两个概念的混淆问题。

  ```cpp
  std::string str = "Hello world."; 
  std::vector<std::string> v;
  
  // 将使用 push_back(const T&), 即产生拷贝行为 
  v.push_back(str); 
  // 将输出 "str: Hello world."
  std::cout << "str: " << str << std::endl;
  
  // 将使用 push_back(const T&&), 不会出现拷贝行为 
  // 而整个字符串会被移动到 vector 中，所以有时候 std::move 会用来减少拷贝出现的开销 
  // 这步操作后, str 中的值会变为空 
  v.push_back(std::move(str)); 
  // 将输出 "str: " 
  std::cout << "str: " << str << std::endl;
  ```

  

- 完美转发：std::forward, 具体参考pdf，有点难 :dizzy:

  > 在使用循环语句时 `auto &&` 是最安全的方式，因为当 `auto` 被推导为不同的左右引用时，与 `&&` 的坍缩组合是完美转发

  

### 4. 容器

#### 4.1 线性容器

- std::array

  与 std::vector 不同，std::array 对象的大小是固定的，如果容器大小是固定的，那么可以优先考虑使用 std::array 容器。另外由于 std::vector 是自动扩容的，当存入大量的数据后，并且对容器进行了删除操作，容器并不会自动归还被删除元素相应的内存，这时候就需要手动 运行 shrink_to_fit() 释放这部分内存

- std::forward_list

  std::forward_list 是一个列表容器，和 std::list 类似，区别是前者使用单向链表，后者使用双向链表，它是标准库容器中唯一一个不提供 size() 方法的容器

  

#### 4.2 无序容器

> 传统 C++ 中的有序容器 std::map/std::set，这些元素内部通过红黑树进行实现， 插入和搜索的平均复杂度均为 O(log(size))。在插入元素时候，会根据 < 操作符比较元素大小并判断 元素是否相同，并选择合适的位置插入到容器中。当对这个容器中的元素进行遍历时，输出结果会按照 < 操作符的顺序来逐个遍历。

无序容器中的元素是不进行排序的， 内部通过 Hash 表实现， 插入和搜索元素的平均复杂度为 O(constant)，在不关心容器内部元素顺序时，能够获得显著的性能提升。

C++ 11新引入两组无序容器：

- std::unordered_map/std::unordered_multimap
- std::unordered_set/std::unordered_multiset

  

#### 4.3 元组

**基本操作**

纵观传统 C++ 中的容器，除了 std::pair 外，似乎没有现成的结构能够用来存放不同类型的数据（通常我们会自己定义结构）。但 std::pair 的缺点是只能保存两个元素

C++11 引入 std::tuple, 他有三个核心函数：

- std::make_tuple: 构造元组，`student = std::make_tuple(1.7, ’D’, " 王五")`

- std::get: 获得元组某个位置的值，`std::get<0>(student) == 1.7`

  > C++ 14 还引入使用类型来获取元组对象，类似于`std::get<std::string>(student) == " 王五"`

- std::tie: 元组拆包，`std::tie(gpa, grade, name) = stduent`

  

**运行期索引**

通过使用 std::variant<>（C++ 17 引入），提供给 variant<> 的类型模板 参数可以让一个 variant<> 从而容纳提供的几种类型的变量

```cpp
// tuple_index 是自己定义的函数，参考原pdf
int i = 1; 
std::cout << tuple_index(t, i) << std::endl;	// i 是索引变量
```

  

**元组合并与遍历**

- 合并

  ```cpp
  // student 和 t 是两个不同的元组
  auto new_tuple = std::tuple_cat(student, std::move(t));
  ```

- 遍历

  ```cpp
  template <typename T>
  auto tuple_len(T &tpl) {
      return std::tuple_size<T>::value;
  }
  for(int i = 0; i != tuple_len(new_tuple); ++i) {
    	std::cout << tuple_index(new_tuple, i) << std::endl;
  }
  ```



### 5. 智能指针与内存管理

#### 5.1 RAII与引用计数

在传统 C++ 中，『记得』手动释放资源，总不是最佳实践。因为我们很有可能就忘记了去释放资源 而导致泄露。所以通常的做法是对于一个对象而言，我们在构造函数的时候申请空间，而在析构函数（在离开作用域时调用）的时候释放空间，也就是我们常说的 RAII (Resource Acquisition is Initialization) 资源获取即初始化技术。

C++11 引入智能指针，使用引用计数的思想，头文件为 \<memory\>，包括std::shared_ptr/std::unique_ptr/std::weak_ptr

> 引用计数不是垃圾回收，引用计数能够尽快收回不再被使用的对象，同时在回收的过程中也不会造成长时间的等待，更能够清晰明确的表明资源的生命周期

  

#### 5.2 std::shared_ptr

- 智能指针是个模板类，可以指定类型，传入指针通过构造函数初始化，也可以使用make_shared函数初始化

```cpp
#include <iostream>
#include <memory>

int main () {
    std::shared_ptr<int> foo = std::make_shared<int> (10);
    // same as:
    std::shared_ptr<int> foo2 (new int(10));

    auto bar = std::make_shared<int> (20);
    auto baz = std::make_shared<std::pair<int,int>> (30,40);

    std::cout << "*foo: " << *foo << '\n'; // 10
    std::cout << "*bar: " << *bar << '\n'; // 20
    std::cout << "*baz: " << baz->first << ' ' << baz->second << '\n'; // 30 40

    return 0;
}
```

- std::shared_ptr 可以通过 get() 方法来获取原始指针，通过 reset() 来减少一个引用计数，并通过 use_count() 来查看一个对象的引用计数。
- 根据 [cppreference](https://cplusplus.com/reference/memory/shared_ptr/)：`shared_ptr` 对象可以共享指针的所有权，同时指向另一个对象。这种能力被称为 `aliasing`，通常用于指向成员对象，同时拥有它们所属的对象。因此，shared_ptr 可能与两个指针有关：① **stored pointer**: is said to point to, and the one it dereferences with operator; ② **owned pointer**: is the pointer the ownership group is in charge of  deleting at some point

代码参考：[shared_ptr.cpp](./shared_ptr.cpp)

- 构造函数也是 explicit 的，没有 delete 拷贝构造和赋值构造函数，所以可以共享
- 指向资源的指针增加了一个引用计数就加 1，指向资源的指针减少了一个引用计数就加 1
- 可以使用 move 将 share_ptr 抓换成 unique_ptr，反之不行
- **删除器：** 可以自定义删除器，可以为普通函数、仿函数和 Lambda 表达式



#### 5.3 std::unique_ptr

std::unique_ptr 是一种独占的智能指针，它禁止其他智能指针与其共享同一个对象，从而保证代码的安全：

```cpp
// make_unique 从 C++14 引入
std::unique_ptr<int> pointer = std::make_unique<int>(10); 
std::unique_ptr<int> pointer2 = pointer; // 非法
```

既然是独占， 换句话说就是不可复制。 但是， 我们可以利用 std::move 将其转移给其他的 unique_ptr，例如：[unique_ptr.cpp](./unique_ptr.cpp)

- 构造函数是 explicit 的，所以不能简单的赋值
- 禁用了拷贝构造函数和拷贝赋值函数，所以就实现了独享语义，unique_ptr 就是想用一个智能指针管理对象
- 不要用同一个裸指针初始化多个 unique_ptr
- release() 可以释放 unique_ptr 对原始指针的控制权，返回原始指针，get() 仅仅是返回原始指针
- 程序使用 exit() 退出时，全局的 unique_ptr 可以正常释放，但是局部的 unique_ptr 不会自动释放



**注意📢：一般来说，unique_ptr 比 shared_ptr 性能更好**

`sizeof(unique_ptr<T>) = sizeof(void*)`, `sizeof(shared_ptr<T>) = sizeof(weak_ptr<T>) = sizeof(void*)*2`，前者能做到几乎零花销，很多情况下开-O1能做到跟new-delete相同的效率，且**能保证异常安全**；后者多出来一个指向引用计数控制块的指针，当不使用`make_shared`时，两块资源的内存是分开的，即会多一次内存分配。
日常开发很少需要资源共享，只是需要实现 RAII 而已，使用`unique_ptr`足矣。

参考：https://github.com/LeetCode-Feedback/LeetCode-Feedback/issues/8693#issuecomment-1218950002



#### 5.4 std::weak_ptr

weak_ptr 是一种不控制对象生命周期的智能指针, 它指向一个 shared_ptr 管理的对象。进行该对象的内存管理的是强引用的 shared_ptr，weak_ptr 只是提供了对管理对象的一个访问手段。weak_ptr 设计的目的是为配合 shared_ptr 而引入的一种智能指针来协助 shared_ptr 工作, 它只可以从一个 shared_ptr 或另一个 weak_ptr 对象构造, **它的构造和析构不会引起引用记数的增加或减少，通常用来打破 shared_ptr 的循环计数**

std::weak_ptr 没有 `*` 运算符和 `->` 运算符，所以不能够对资源进行操作，它的唯一作用就是用于 检查 std::shared_ptr 是否存在，其 expired() 方法能在资源未被释放时，会返回 false，否则返回 true。

代码参考：[weak_ptr.cpp](./weak_ptr.cpp)

- weak_ptr 不控制对象的生命周期，但是它知道对象是否还活着
- 用 lock() 函数把它可以提升为shared_ptr，如果对象还活着，返回有效的shared_ptr，如果对象已经死了，提升会失败，返回一个空的shared ptr。
- 提升的行为 lock() 是线程安全的



#### 总结 :fire:

##### 不带引用计数

auto_ptr 是原 C++ 库就存在，现在应该过时了，scope_ptr 和 unique_ptr 是 C++11 新标准

- auto_ptr：拷贝构造或者赋值构造会直接将原指针置空

- scope_ptr

  ```cpp
  scoped_ptr(const scoped_ptr<T>&) = delete;
  scoped_ptr<T>& operator=(cosnt scoped_ptr<T>&) = delete;
  ```

- unique_ptr

  ```cpp
  unique_ptr(const unique_ptr<T>&) = delete;
  unique_ptr<T>& operator=(cosnt unique_ptr<T>&) = delete;
  
  // 提供右值引用的拷贝构造和赋值
  unique_ptr(unique_ptr<T>&&)
  unique_ptr<T>& operator=(unique_ptr<T>&&)
  ```



##### 带引用计数

多个智能指针可以管理同一个资源

定义对象的时候使用强智能指针，引用对象的时候使用弱智能指针

- shared_ptr: 强智能指针可以改变引用计数，:fire:自定义 [CSmartPtr.cpp](smart_ptr.cpp)

- weak_ptr: 弱智能指针只起到监测的作用

```cpp
// 解决线程安全问题
class A {
  public:
    A() { cout << "A()\n"; }
    ~A() { cout << "~A()\n"; }
    void testA() { cout << "非常好用的方法！\n"; }
};

void handler(weak_ptr<A> p) {
    shared_ptr<A> sp = p.lock();
    if (sp != nullptr) {
        sp->testA();
    } else {
        cout << "A 对象已经析构，不能再访问了！\n";
    }
}

int main() {
    {
        std::shared_ptr<A> p(new A());
        std::thread t(handler, weak_ptr<A>(p));
        t.detach();
    }
    std::this_thread::sleep_for(std::chrono::seconds(4));
    return 0;
}
```



##### make_shared 和 make_unique

创建引用计数的智能指针 shared_ptr，包括 T * 以及 RefCnt *ref，并且 RefCnt 包括两个原子类型的参数 use 和 weak（底层通过 CAS 实现，是线程安全的）

```cpp
shared_ptr<int> sp1(new int(10)); // 引用对象和引用计数是分开 new 的，有时候一个 new 失败
shared_ptr<int> sp2 = make_shared<int>(10); // 引用对象和引用计数一起 new，要么成功要么失败
```

make_shared 优点

- 内存分配效率高了，因为引用对象以及引用计数是一起的 new 的
- 防止了资源泄露的风险，因为引用对象和引用计数要么 new 成功要么失败

make_shared 缺点

- make_shared 无法自定义删除器，类模板没有提供参数
- 导致托管的资源延迟释放，因为 shared_ptr new 只要等到 uses == 0 就会释放资源，不管有没有观察对象 weak，make_shared 是需要看 uses 和 weak 的，只有 uses 和 weak 都为 0 时才释放资源



### 6. 正则表达式

#### 6.1 简介

正则表达式是由**普通字符**（例如 a 到 z）以及**特殊字符**组成的文字模式。模式描述在搜索文本时要匹配的一个或多个字符串。正则表达式作为一个模板，将某个字符模式与所搜索的字符串进行匹配。一般正则表达式用于：

- 检查子串
- 替换子串
- 获取子串

关于普通字符和特殊字符参考《modern-cpp-tutorial-zh.pdf》

  

#### 6.2 std::regex

C++11 提供的正则表达式库操作 std::string 对象，模式 std::regex (本质是 `std::basic_regex`) 进行初始化，通过 std::regex_match 进行匹配，从而产生 std::smatch（本质是 `std::match_results` 对象）

另 一 种 常 用 的 形 式 就 是 依 次 传 入 `std::string/std::smatch/std::regex` 三 个 参 数， 其 中 std::smatch 的本质其实是 `std::match_results`。 在标准库中，std::smatch 被定义为了 `std::match_results<std::string::const_iterator>`，也就是一个子串迭代器类型的 match_results。 使用 std::smatch 可以方便的对匹配的结果进行获取，例如：

代码参考：[regex.cpp](./regex.cpp)

  

### 7. 并行与并发

#### 7.1 并行基础

std::thread 用于创建一个执行的线程实例， 所以它是一切并发编程的基础， 使用时需要包含 `<thread>` 头文件，它提供了很多基本的线程操作，例如 get_id() 来获取所创建线程的线程 ID，使用 join() 来加入一个线程等等，例如：

```cpp
std::thread t([](){ 
  std::cout << "hello world." << std::endl;
}); 
t.join();
```

  

#### 7.2 互斥量与临界区

std::mutex 是 C++11 中最基本的 mutex 类，通过实例化 std::mutex 可以创建互斥量，而通过其 成员函数 lock() 可以进行上锁，unlock() 可以进行解锁。但是在实际编写代码的过程中，最好不去直 接调用成员函数，因为调用成员函数就需要在每个临界区的出口处调用 unlock()，当然，还包括异常。 这时候 C++11 还为互斥量提供了一个 RAII 语法的模板类 std::lock_guard。RAII 在不失代码简洁性 的同时，很好的保证了代码的异常安全性。

> 代码参考：[lock_guard.cpp](./lock_guard.cpp)

std::unique_lock 则相对于 std::lock_guard 出现的，std::unique_lock 更加灵活， std::unique_lock 的对象会以独占所有权（没有其他的 unique_lock 对象同时拥有某个 mutex 对象的所有权）的方式管理 mutex 对象上的上锁和解锁的操作。 所以在并发编程中， 推荐使用 std::unique_lock。

std::lock_guard 不能显式的调用 lock 和 unlock，而 std::unique_lock 可以在声明后的任意位置调用，可以缩小锁的作用范围，提供更高的并发度。

如果你用到了条件变量 std::condition_variable::wait 则必须使用 std::unique_lock 作为参数。

> 代码参考：[unique_lock.cpp](./unique_lock.cpp)

  

#### 7.3 期物

期物（Future）表现为 std::future，它提供了一个访问异步操作结果的途径，这句话很不好理解。 为了理解这个特性，我们需要先理解一下在 C++11 之前的多线程行为。

> 试想，如果我们的主线程 A 希望新开辟一个线程 B 去执行某个我们预期的任务，并返回我一个结 果。而这时候，线程 A 可能正在忙其他的事情，无暇顾及 B 的结果，所以我们会很自然的希望能够在某 个特定的时间获得线程 B 的结果。

在 C++11 的 std::future 被引入之前，通常的做法是：创建一个线程 A，在线程 A 里启动任务 B，当准备完毕后发送一个事件，并将结果保存在全局变量中。而主函数线程 A 里正在做其他的事情，当需要结果的时候，调用一个线程等待函数来获得执行的结果。

而 C++11 提供的 std::future 简化了这个流程，可以用来获取异步任务的结果。自然地，我们很 容易能够想象到把它作为一种简单的线程同步手段，即屏障（barrier）。

代码参考：[future.cpp](./future.cpp)

  

#### 7.4 条件变量

条件变量 std::condition_variable 是为了解决死锁而生，当互斥操作不够用而引入的。比如，线程可能需要等待某个条件为真才能继续执行，而一个忙等待循环中可能会导致**所有其他线程都无法进入临界区**使得条件为真时，就会发生死锁。所以，condition_variable 实例被创建出现主要就是用于唤**醒等待线程从而避免死锁**。std::condition_variable 的 notify_one() 用于唤醒一个线程；notify_all() 则是通知所有线程。

代码参考：[condition_var.cpp](./condition_var.cpp)



#### 7.5 原子操作与内存模型

- 原子操作：`atomic<int> counter`，具体[参考](https://changkun.de/modern-cpp/zh-cn/07-thread/#原子操作)
- 一致性模型
  - 线性一致性：又称**强一致性**或**原子一致性**。它要求任何一次读操作都能读到某个数据的最近一次写 的数据，并且所有线程的操作顺序与全局时钟下的顺序是一致的
  - 顺序一致性：同样要求任何一次读操作都能读到数据最近一次写入的数据，**但未要求与全局时钟的 顺序一致**
  - 因果一致性：它的要求进一步降低，**只需要有因果关系的操作顺序得到保障**，而非因果关系的操作 顺序则不做要求
  - 最终一致性：是最弱的一致性要求，它只保障某个操作在未来的某个时间节点上会被观察到，但并 未要求被观察到的时间。
- 内存顺序
  - 宽松模型
  - 释放/消费模型
  - 释放/获取模型
  - 顺序一致模型



### 8. 文件系统

文件系统库提供了文件系统、路径、常规文件、目录等等相关组件进行操作的相关功能。和正则表达式库类似，他也是最先由 boost 发起，并最终被合并为 C++ 标准的众多库之一。

#### 8.1 文档与链接

todo

  

#### 8.2 std::filesystem  

todo

  

### 9. 其他杂项

#### 9.1 新类型

long long int 并不是 C++11 最先引入的，其实早在 C99，long long int 就已经被纳入 C 标 准中，所以大部分的编译器早已支持。C++11 的工作则是正式把它纳入标准库，规定了一个 long long int 类型至少具备 64 位的比特数。



#### 9.2 nonexcept

C++11 将异常的声明简化为以下两种情况：

1. 函数可能抛出任何异常
2. 函数不能抛出任何异常

并使用 noexcept 对这两种行为进行限制，例如：

```cpp
void may_throw(); // 可能抛出异常 
void no_throw() noexcept; // 不可能抛出异常
```

使用 noexcept 修饰过的函数如果抛出异常，编译器会使用 std::terminate() 来立即终止程序运行。

noexcept 还能够做操作符，用于操作一个表达式，当表达式无异常时，返回 true，否则返回 false。

  

#### 9.3 字面量

- 原始字符串字面量

  传统 C++ 里面要编写一个充满特殊字符的字符串其实是非常痛苦的一件事情， 比如一个 包含 HTML 本体的字符串需要添加大量的转义符， 例如一个 Windows 上的文件路径经常会： C:\\File\\To\\Path。

  C++11 提供了原始字符串字面量的写法，可以在一个字符串前方使用 R 来修饰这个字符串，同时， 将原始字符串使用括号包裹，例如：

  ```cpp
  std::string str = R"(C:\File\To\Path)"; 
  std::cout << str << std::endl;
  ```

  

- 自定义字面量

  C++11 引进了自定义字面量的能力，通过重载双引号后缀运算符实现，需要注意参数列表，代码参考：[R_string.cpp](./R_string.cpp)

  自定义字面量支持四种字面量：

  1. 整型字面量：重载时必须使用 `unsigned long long`、`const char *`、模板字面量算符参数

  2. 浮点型字面量：重载时必须使用 `long double`、`const char *`、模板字面量算符；

  3. 字符串字面量：必须使用 (`const char *`, `size_t`) 形式的参数表；

  4. 字符字面量：参数只能是 `char`, `wchar_t`, `char16_t`, `char32_t` 这几种类型。

  

#### 9.4 内存对齐

C++ 11 引入了两个新的关键字 alignof 和 alignas 来支持对内存对齐进行控制。alignof 关键字能够获得一个与平台相关的 std::size_t 类型的值，用于查询该平台的对齐方式。当然我们有时候并不满足于此，甚至希望自定定义结构的对齐方式，同样，C++ 11 还引入了 alignas 来重新修饰某个结构的对齐方式。

参考代码：[alignof.cpp](./alignof.cpp)

  

### 10. C++20 简介

todo
