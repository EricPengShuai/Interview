## 罕见知识

### C++ 关键字

#### 1、extern "C"作用

为了能够**正确实现C++代码调用其他C语言代码**。加上extern “C”后，会指示编译器对这部分代码按照C语言的特性进行编译。**最终实现C++与C及其他语言的混合编程**

由于C++支持函数重载，因此编译器编译函数的过程中会将函数的参数类型也加到编译之后的代码中，而不仅仅是函数名；而C语言并不支持函数重载，因此**编译C语言代码的函数时不会带上函数的参数类型**，一般只是包括函数名

> 比如说你用C 开发了一个DLL 库，为了能够让C ++语言也能够调用你的DLL输出(Export)的函数，你需要用extern "C"来强制编译器不要修改你的函数名。

- **Extern关键字**

表明函数和全局变量作用范围（可见性）的关键字，其声明的函数和变量可以在所有模块中使用

通常，在模块的头文件中对本模块提供给其它模块引用的函数和全局变量以关键字extern声明。例如，如果模块B欲引用该模块A中定义的全局变量和函数时只需包含模块A的头文件即可。这样，模块B中调用模块A中的函数时，在编译阶段，模块B虽然找不到该函数，但是并不会报错；它会在**链接阶段**中从模块A编译生成的目标代码中找到此函数。

与extern对应的关键字是static，被它修饰的全局变量和函数只能在本模块中使用。因此，一个函数或变量只可能被本模块使用时，其不可能被extern “C”修饰。

- **被extern “C"修饰的变量和函数是按照C语言方式编译和链接的**

作为一种面向对象的语言，C++支持函数重载，而过程式语言C则不支持。

函数被C++编译后在符号库中的名字与C语言的不同。例如，假设某个函数的原型为：`void foo( int x, int y )`; 该函数被C编译器编译后在符号库中的名字为`_foo`，而C++编译器则会产生像`_foo_int_int`之类的名字（不同的编译器可能生成的名字不同，但是都采用了相同的机制，生成的新名字称为“mangled name”）。

参考：https://www.jianshu.com/p/5d2eeeb93590



#### 2、thread_local

*thread_local* 关键词只对声明于命名空间作用域的对象、声明于块作用域的对象及静态数据成员允许。它指示对象拥有线程存储期。它能与 static 或 extern 结合，以分别指定内部或外部链接（除了静态数据成员始终拥有外部链接），但附加的 static 不影响存储期。 

> **线程存储期**：对象的存储在线程开始时分配，而在线程结束时解分配。每个线程拥有其自身的对象实例。唯有声明为 thread_local 的对象拥有此存储期。 thread_local 能与 static 或 extern 一同出现，以调整链接

**要点：**

- thread_local 描述的对象**在 thread 开始时分配，而在 thread 结束时分解**
- 一般在声明时赋值，在本 thread 中只**执行一次**
- 描述的对象依然只在**作用域内有效**
- **描述类成员变量时，必须是 static 的**



##### - 全局变量 vs 全部变量

```cpp
#include <iostream>
#include <thread>
#include <mutex>
std::mutex cout_mutex;    // 多线程互斥打印

// 全局变量
thread_local int x = 1;
void thread_func_global(const std::string& thread_name) {
    for (int i = 0; i < 3; ++i) {
        x++;
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "thread[" << thread_name << "]: x = " << x << std::endl;
    }
    return;
}

// 局部变量
void thread_func_local(const std::string& thread_name) {
    for (int i = 0; i < 3; ++i) {
        thread_local int x = 1;
        x++;
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "thread[" << thread_name << "]: x = " << x << std::endl;
    }
    // x++; // 编译会出错：error: ‘x’ was not declared in this scope
    return;
}

int main() {
    std::thread t1(thread_func_global, "t1");
    std::thread t2(thread_func_global, "t2");
    t1.join();
    t2.join();
    return 0;
}
```

输出结果都是：（注意有些顺序是随机的）

```bash
thread[t2]: x = 2
thread[t2]: x = 3
thread[t2]: x = 4
thread[t1]: x = 2
thread[t1]: x = 3
thread[t1]: x = 4
```

**说明：**

- 可以看出 x 在**每个线程里是分别自加互不干扰的**

- thread_local 的局部变量没有因为 for 循环作用域而重新赋值。这是因为**线程存储期的变量都是和线程绑定的**，所以只有第一次声明时被赋值。可以理解为**线程的 static 变量**。不过变量的作用域依然是在本身的作用域内，如果在 for 循环外使用 x 就会编译时错误



##### - 类对象

```cpp
#include <iostream>
#include <thread>
#include <mutex>
std::mutex cout_mutex;

// 测试类
class A {
public:
    A() {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "create A" << std::endl;
    }

    ~A() {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "destroy A" << std::endl;
    }

    int counter = 0;
    int get_value() {
        return counter++;
    }
};

void thread_func(const std::string& thread_name) {
    for (int i = 0; i < 3; ++i) {
        thread_local A* a = new A();
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "thread[" << thread_name << "]: a.counter:" 
            	  << a->get_value() << std::endl;
    }
    return;
}

int main() {
    std::thread t1(thread_func, "t1");
    std::thread t2(thread_func, "t2");
    t1.join();
    t2.join();
    return 0;
}
```

输出结果：（注意有些顺序是随机的）

```bash
create A
thread[t1]: a.counter:0
thread[t1]: a.counter:1
thread[t1]: a.counter:2
create A
thread[t2]: a.counter:0
thread[t2]: a.counter:1
thread[t2]: a.counter:2
```

**说明**

- 可以看出虽然在循环中创建了 A 的实例 a，但是并没有因为循环创建了多个。这个与局部变量的情况相同，创建的实例相对于 thread 是 static 的。

- 一般情况下，**thread_local 对象最好在声明时赋值**，如果声明之后赋值，for 循环中使用每次都会创建一个 实例



##### - 类成员

```cpp
#include <iostream>
#include <thread>
#include <mutex>
std::mutex cout_mutex;

class B {
public:
    B() {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "create B" << std::endl;
    }
    ~B() {}
    thread_local static int b_key;
    // thread_local int b_key;
    int b_value = 24;
    static int b_static;
};

// 静态成员变量必须在类外初始化
int B::b_static = 36;
thread_local int B::b_key = 12;

void thread_func(const std::string& thread_name) {
    B b;
    for (int i = 0; i < 3; ++i) {
        b.b_key--;
        b.b_value--;    // not thread safe
        b.b_static--;   // not thread safe 
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "thread[" << thread_name << "]: b_key:" << b.b_key << ", b_value:" << b.b_value << ", b_static:" << b.b_static << std::endl;
        std::cout << "thread[" << thread_name << "]: B::key:" << B::b_key << ", b_value:" << b.b_value << ", b_static: " << B::b_static << std::endl;
    }
    return;
}

int main() {
    std::thread t1(thread_func, "t1");
    std::thread t2(thread_func, "t2");
    t1.join();
    t2.join();
    return 0;
}
```

输出：（注意有些顺序是随机的）

```bash
create B
thread[t1]: b_key:11, b_value:23, b_static:35
thread[t1]: B::key:11, b_value:23, b_static: 35
thread[t1]: b_key:10, b_value:22, b_static:34
thread[t1]: B::key:10, b_value:22, b_static: 34
thread[t1]: b_key:9, b_value:21, b_static:33
thread[t1]: B::key:9, b_value:21, b_static: 33
create B
thread[t2]: b_key:11, b_value:23, b_static:32
thread[t2]: B::key:11, b_value:23, b_static: 32
thread[t2]: b_key:10, b_value:22, b_static:31
thread[t2]: B::key:10, b_value:22, b_static: 31
thread[t2]: b_key:9, b_value:21, b_static:30
thread[t2]: B::key:9, b_value:21, b_static: 30
```

**说明：**

- thread_local 作为类成员变量时必须是 static 的
- 可以看出 thread_local 作为类成员时也是**对于每个 thread 分别分配了一个**。而 static 则是全局一个



参考：

- [C++11 thread_local用法 - 此方一泉](https://zhuanlan.zhihu.com/p/340201634)



#### 3、lock_guard vs unique_lock

首先这个都是 non-copyable，但是 unique_lock 可以转移（提供了移动构造和移动赋值函数，类似于 unique_ptr），lock_guard 只能在构造 lock、在析构时 unlock

参考 stackoverflow 几个高赞回答

- 除非你需要在不销毁锁的情况下手动解锁互斥锁，否则请使用 lock_guard，**也就是说一般情况下不需要手动释放 mutex，lock_guard 足矣**
- 特别是，当调用 wait 时，condition_variable 会在进入睡眠时解锁其互斥锁。这就是为什么在这里lock_guard 不足够的原因。 C++17 之后，请考虑使用 scoped_lock 替代 lock_guard

- **unique_lock 可以在构造时不立即 lock 互斥锁**，结合 std::defer_lock 可以实现 RAII wrapper（锁住多个 mutex），lock_guard 也能提供 RAII wrapper（使用括弧限制一个 limited space）

  ```cpp
  // unique_lock 提供的 RAII wrapper
  void transfer(Box &from, Box &to, int num)
  {
      // don't actually take the locks yet
      std::unique_lock lock1{from.m, std::defer_lock};
      std::unique_lock lock2{to.m, std::defer_lock};
   
      // lock both unique_locks without deadlock
      std::lock(lock1, lock2);
   
      from.num_things -= num;
      to.num_things += num;
   
      // 'from.m' and 'to.m' mutexes unlocked in 'unique_lock' dtors
  }
  
  // lock_guard 提供的 RAII wrapper
  class MyClass{
      std::mutex my_mutex;
      void member_foo() {
          std::lock_guard<mutex_type> lock(this->my_mutex);            
          /*
           block of code which needs mutual exclusion (e.g. open the same 
           file in multiple threads).
          */
  
          //mutex is automatically released when lock goes out of scope
      }           
  };
  ```

  

**参考**

- lock_guard: https://en.cppreference.com/w/cpp/thread/lock_guard
- unique_lock: https://en.cppreference.com/w/cpp/thread/unique_lock
- stackoverflow: [std::unique_lock or std::lock_guard?](https://stackoverflow.com/questions/20516773/stdunique-lockstdmutex-or-stdlock-guardstdmutex)
- [What's the best way to lock multiple std::mutex'es?](https://stackoverflow.com/questions/17113619/whats-the-best-way-to-lock-multiple-stdmutexes)
