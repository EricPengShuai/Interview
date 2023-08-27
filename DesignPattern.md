## 设计模式

### 1. 单例模式

单例模式保证一个类只有一个实例，并且提供一个访问该全局访问点

实现原理就是将能够创建对象的函数都设置为 private，通过**静态成员**返回一个实例，具体而言：

1. 构造函数或者拷贝构造函数私有化

2. 定义一个唯一的类的实例对象
3. 获取类的唯一实例对象的结构方法

**应用：**

- 网站计数器，单例模式容易实现同步
- 日志模块，应用程序一般是需要维护一个日志实例，否则内容不好追加显示



#### 1.1 懒汉式

懒汉模式是在**第一次用到类实例时**才会实例化对象。类初始化时，不会初始化该对象，真正需要使用的时候才会创建该对象，具备懒加载功能

```cpp
class CSingleton {
  public:
    // #3 获取类的唯一实例对象的结构方法
    static CSingleton *getInstance() {
        if (instance == NULL) {
            //!NOTE: 锁+双重判断 -> 线程安全
            lock_guard<std::mutex> guard(mtx);
            if (instance == NULL) {
                instance = new CSingleton();
            }
        }
        return instance;
    };

  private:
    static std::mutex mtx;
    // #2 定义一个唯一的类的实例对象
    //!NOTE: 在多线程情况下让各个线程立即看到 instance 的改变，直接读取内存，而不是从缓存中拿
    static CSingleton *volatile instance;

    // #1 涉及创建对象的函数都设置为private
    CSingleton() {}
    ~CSingleton() { cout << "~CSingleton()" << endl; }
    CSingleton(const CSingleton &s) = delete;
    CSingleton& operator()(const CSingleton &s) = delete;

    //!NOTE: 定义一个嵌套类，在该类的析构函数中，自动释放外层类的资源
    class CRelease {
      public:
        ~CRelease() { delete instance; }
    };
    // 通过该静态对象在程序结束时自动析构的特点，来释放外层类的对象资源
    static CRelease release;
};

// 注意这里需要先声明
CSingleton *volatile CSingleton::instance = nullptr;
std::mutex CSingleton::mtx;
CSingleton::CRelease CSingleton::release;
```

- 写法2：C++11 之后编译器可以保证「局部静态变量」的线程安全性，**对于static静态局部变量的初始化，编译器会自动对它的初始化进行加锁和解锁控制，使静态局部变量的初始化成为线程安全的操作，不用担心多个线程都会初始化静态局部变量，因此下面的懒汉单例模式是线程安全的单例模式！**

```cpp
class CSingleton {
  public:
    static CSingleton *getInstance() {
        static CSingleton instance;
        return &instance;
    };
    ~CSingleton(){};

  private:
    CSingleton(){};
    CSingleton(const CSingleton &s) = delete;
    CSingleton& operator()(const CSingleton &s) = delete;
};
```



#### 1.2 饿汉式

**饿汉式**单例模式，顾名思义，就是程序启动时就实例化了该对象，并没有推迟到第一次使用该对象时再进行实例化；如果运行过程中没有使用到，该实例对象就被浪费掉了。类初始化时，会立即加载该对象，**线程天生安全**，调用效率高

```cpp
class CSingleton {
  public:
    static CSingleton *getInstance() { return &single; }

  private:
    static CSingleton single;
    CSingleton() { cout << "CSingleton()" << endl; }
    ~CSingleton() { cout << "~CSingleton()" << endl; }
    CSingleton(const CSingleton &) = delete;
    CSingleton &operator()(const CSingleton &s) = delete;
};

CSingleton CSingleton::single;

int main() {
    CSingleton *p1 = CSingleton::getInstance();
    CSingleton *p2 = CSingleton::getInstance();
    CSingleton *p3 = CSingleton::getInstance();
    cout << p1 << " " << p2 << " " << p3 << endl;  // p1 p2 p3 地址是一样的
    return 0;
}
```

#### 参考

- [C++设计模式 - 单例模式](https://blog.csdn.net/QIANGWEIYUAN/article/details/88544524)
- [最新版Web服务器项目详解 - 09 日志系统（上）](https://mp.weixin.qq.com/s/IWAlPzVDkR2ZRI5iirEfCg)



### 2. 工厂模式

它提供了一种创建对象的最佳方式。在工厂模式中，我们在创建对象时不会对客户端暴露创建逻辑，并且是通过使用一个共同的接口来指向新创建的对象。实现了**创建者和调用者分离**，工厂模式分为简单工厂、工厂方法、抽象工厂模式



#### 2.1 简单工厂

简单工厂模式的实质是由一个工厂类**根据传入的参数**，动态决定应该创建哪一个产品类（这些产品类继承自一个父类或接口）的实例

```cpp
#include <iostream>
#include <pthread.h>
using namespace std;

// 产品类（抽象类，不能实例化）
class Product {
public:
    Product(){};
    virtual void show()=0;  // 纯虚函数
    virtual ~Product(){};
};

class productA: public Product {
public:
    productA() {};
    void show() { cout << "product A create!" << endl; };
    ~productA() { cout << "product A delete!" << endl; };
};

class productB: public Product {
public:
    productB() {};
    void show() { cout << "product B create!" << endl; };
    ~productB() { cout << "product B delete!" << endl; };
};

class simpleFactory { // 工厂类
public:
    simpleFactory() {};
    Product* product(const string str) {
        if (str == "productA")
            return (new productA());
        if (str == "productB")
            return (new productB());
        return NULL;
    };
    ~simpleFactory(){};
};

int main(){
    simpleFactory obj;	// 创建工厂
    Product* pro;	// 创建产品

    pro = obj.product("productA");
    pro->show();	// product A create!
    delete pro;

    pro = obj.product("productB");
    pro->show();	// product B create!
    delete pro;

    return 0;
}
```

工厂模式为的就是**代码解耦**，如果我们不采用工厂模式，如果要创建产品A、B，我们通常做法是不是用switch...case语句？那麻烦了，代码耦合程度高，后期添加更多的产品进来，我们不是要添加更多的case吗？这样就太麻烦了，而且不符合设计模式中的**开放封闭原则**。

#### 2.2 抽象工厂

为了进一步解耦，在简单工厂的基础上发展出了抽象工厂模式，**即连工厂都抽象出来**，实现了进一步代码解耦。代码如下：

```cpp
#include <iostream>
#include <pthread.h>
using namespace std;

// 产品类（抽象类，不能实例化）
class Product{
public:
    Product(){}
    virtual void show()=0;  // 纯虚函数
    virtual ~Product(){}
};

// 产品A
class ProductA: public Product{
public:
    ProductA(){}
    void show(){ cout<<"product A create!"<<endl; };
    ~ProductA(){ cout<<"product A delete!"<<endl; };
};

// 产品B
class ProductB: public Product{
public:
    ProductB(){}
    void show(){ cout<<"product B create!"<<endl; };
    ~ProductB(){ cout<<"Product B delete!"<<endl; };
};

class Factory{ // 抽象类
public:
    virtual Product* CreateProduct()=0;
    ~Factory(){}
};

class FactorA: public Factory{ // 工厂类A，只生产A产品
public:
    Product* CreateProduct(){
        return (new ProductA());
    }
};

class FactorB: public Factory{ // 工厂类B，只生产B产品
public:
    Product* CreateProduct(){
        return (new ProductB());
    }
};

int main(){
    Product* _Product = nullptr;
    auto MyFactoryA = new FactorA();
    _Product = MyFactoryA->CreateProduct(); // 调用产品A的工厂来生产A产品
    _Product->show();
    delete _Product;

    auto MyFactoryB = new FactorB();
    _Product = MyFactoryB->CreateProduct(); // 调用产品B的工厂来生产B产品
    _Product->show();
    delete _Product;
    
    getchar();
    return 0;
}
```

#### 参考

- [C++设计模式 - 简单工厂，工厂方法和抽象工厂](https://blog.csdn.net/QIANGWEIYUAN/article/details/88792594)



### 3. 观察者模式

观察者模式，是一种行为性模型，又叫发布-订阅模式，定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新。比如拍卖的时候，拍卖师观察最高标价，然后通知给其他竞价者竞价。

> 行为性模型：关注系统中对象之间的相互交互，解决系统在运行时对象之间的相互通信和协作，进一步明确对象的职责。

```cpp
#include <iostream>
#include <list>
using namespace std;
 
class Observer{  // 观察者抽象
public:
    virtual void Update(int) = 0;
};
 
class ConcreteObserver1:public Observer{  // 第一个观察者
public:
    ConcreteObserver1(Subject *pSubject):m_pSubject(pSubject){}
    void Update(int value){
        cout << "ConcreteObserver1 get the update. New State:" << value << endl;
    }
private:
    Subject *m_pSubject;
};
 
class ConcreteObserver2 : public Observer{  // 第二个观察者
public:
    ConcreteObserver2(Subject *pSubject):m_pSubject(pSubject){}
    void Update(int value){
        cout << "ConcreteObserver2 get the update. New State:" << value << endl;
    }
private:
    Subject *m_pSubject;
};

class Subject{  // 被观察者抽象
public:
    virtual void Attach(Observer *) = 0;
    virtual void Detach(Observer *) = 0;
    virtual void Notify() = 0;
};
 
class ConcreteSubject:public Subject{  // 被观察者
public:
    void Attach(Observer *pObserver);
    void Detach(Observer *pObserver);
    void Notify();
    void SetState(int state){
        m_iState = state;
    }
private:
    std::list<Observer *> m_ObserverList;
    int m_iState;
};
 
void ConcreteSubject::Attach(Observer *pObserver){ // 添加观察者
    m_ObserverList.push_back(pObserver);
}
 
void ConcreteSubject::Detach(Observer *pObserver){ // 删除观察者
    m_ObserverList.remove(pObserver);
}
 
void ConcreteSubject::Notify(){  // 通知观察者
    std::list<Observer *>::iterator it = m_ObserverList.begin();
    while (it != m_ObserverList.end()){
        (*it)->Update(m_iState);
        ++it;
    }
}
 
int main(){
    // Create 被观察者
    ConcreteSubject *pSubject = new ConcreteSubject();
 
    // Create 观察者
    Observer *pObserver1 = new ConcreteObserver1(pSubject);
    Observer *pObserver2 = new ConcreteObserver2(pSubject);
 
    // 改变状态
    pSubject->SetState(2);
 
    // 注册观察者
    pSubject->Attach(pObserver1);
    pSubject->Attach(pObserver2);
 
    pSubject->Notify();// 通知观察者
 
    // 删除观察者
    pSubject->Detach(pObserver1);
    pSubject->Detach(pObserver2);
}
```

#### 参考

- [C++设计模式 - 观察者Observer模式](https://blog.csdn.net/QIANGWEIYUAN/article/details/88745835) :fire:



### 参考

- [常见C++设计模式面试题和场景题](https://blog.nowcoder.net/n/85715187642d4f8c8c19161be64892da)
- [设计模式面试题（总结最全面的面试题！！！）](https://juejin.cn/post/6844904125721772039)
- [C++面试题之设计模式及设计原则](https://blog.csdn.net/QIJINGBO123/article/details/108539158)
