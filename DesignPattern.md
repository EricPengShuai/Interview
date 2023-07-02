## 设计模式

### 1. 单例模式

单例模式保证一个类只有一个实例，并且提供一个访问该全局访问点

实现原理就是将能够创建对象的函数都设置为 private，通过**静态成员**返回一个实例，具体而言：

- 构造函数或者拷贝构造函数私有化；
- 共有的静态变量和静态函数；

**应用：**

1. 网站计数器，单例模式容易实现同步
2. 日志模块，应用程序一般是需要维护一个日志实例，否则内容不好追加显示



#### 1.1 懒汉式

懒汉模式是在**第一次用到类实例时**才会实例化对象。类初始化时，不会初始化该对象，真正需要使用的时候才会创建该对象，具备懒加载功能

```cpp
class single {
public:
    static single* getInstance() {
        if (instance == NULL){
            pthread_mutex_lock(&mutex); // mlock.lock();
            if (instance == NULL) {
                instance = new single();
            }
            pthread_mutex_unlock(&mutex); // mlock.unlock();
        }
        return instance;
    };
    ~single(){};
    
private:
    static pthread_mutex_t mutex; // mutex mlock; 加锁互斥
    static single* instance;
    
    // 涉及创建对象的函数都设置为private
    single(){
        pthread_mutex_init(&mutex, NULL);
    };
};

// 注意这里需要先声明
single* single::instance = nullptr;
pthread_mutex_t single::mutex;

int main(){
    // 因为没有办法创建对象，就得采用静态成员函数的方法返回静态成员变量
    single *s = single::getInstance();
    delete s;
    return 0;
}
```

- 写法2：C++11 之后编译器可以保证内部静态变量的线程安全性

```cpp
class single {
  public:
    static single *getInstance() {
        static single instance;
        return &instance;
    };
    ~single(){};

  private:
    single(){};
};

// 需要先声明
pthread_mutex_t single::mutex;
```

- 写法2：C++11 之前还是需要加锁的

```cpp
class single {
  private:
    static pthread_mutex_t mutex;
    single() { pthread_mutex_init(&mutex, NULL); }

  public:
    static single *getInstance() {
        pthread_mutex_lock(&mutex);
        static single obj;
        pthread_mutex_unlock(&mutex);
        return &obj;
    };
    ~single() {}
};

// 需要先声明
pthread_mutex_t single::mutex;
```



#### 1.2 饿汉式

类初始化时，会立即加载该对象，**线程天生安全**，调用效率高

```cpp
class single {
  private:
    static single *p;
    single() {}
    ~single() {}

  public:
    static single *getinstance() { return p; }
};

// 运行时直接初始化
single *single::p = new single();
```



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
 
class Subject{  // 被观察者抽象
public:
    virtual void Attach(Observer *) = 0;
    virtual void Detach(Observer *) = 0;
    virtual void Notify() = 0;
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





### 参考

- [常见C++设计模式面试题和场景题](https://blog.nowcoder.net/n/85715187642d4f8c8c19161be64892da)
- [设计模式面试题（总结最全面的面试题！！！）](https://juejin.cn/post/6844904125721772039)
- [C++面试题之设计模式及设计原则](https://blog.csdn.net/QIJINGBO123/article/details/108539158)
