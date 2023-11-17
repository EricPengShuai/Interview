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
    CSingleton& operator=(const CSingleton &s) = delete;

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
    CSingleton& operator=(const CSingleton &s) = delete;
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
#include <memory>
#include <string>
using namespace std;

// 产品簇手机类型
class ProductA {
  public:
    ProductA(string name) : _name(name) {}
    // 模拟产品对象的一个抽象方法
    virtual void show() = 0;

  protected:
    string _name;
};
// 产品实体类型定义
class XiaomiPhone : public ProductA {
  public:
    XiaomiPhone(string name) : ProductA(name) {}
    void show() { cout << "获取了一个小米手机:" << _name << endl; }
};
// 产品实体类型定义
class HuaweiPhone : public ProductA {
  public:
    HuaweiPhone(string name) : ProductA(name) {}
    void show() { cout << "获取了一个华为手机:" << _name << endl; }
};

// 产品簇智能手环类型
class ProductB {
  public:
    ProductB(string name) : _name(name) {}
    // 模拟产品对象的一个抽象方法
    virtual void show() = 0;

  protected:
    string _name;
};
// 产品实体类型定义
class XiaomiCircle : public ProductB {
  public:
    XiaomiCircle(string name) : ProductB(name) {}
    void show() { cout << "获取了一个小米智能手环设备:" << _name << endl; }
};
// 产品实体类型定义
class HuaweiCircle : public ProductB {
  public:
    HuaweiCircle(string name) : ProductB(name) {}
    void show() { cout << "获取了一个华为智能手环设备:" << _name << endl; }
};

// 抽象工厂，创建通过一个产品簇的设备产品
class AbstractFactory {
  public:
    // 工厂里面创建手机的纯虚函数接口
    virtual ProductA *createPhone() = 0;
    // 工厂里面创建智能手环的纯虚函数接口
    virtual ProductB *createSmartCircle() = 0;
};

// 生产小米产品簇的工厂
class XiaomiFactory : public AbstractFactory {
  public:
    ProductA *createPhone() {
        // 小米工厂肯定生产小米手机
        return new XiaomiPhone("小米x9");
    }
    ProductB *createSmartCircle() {
        // 小米工厂肯定生产小米智能手环
        return new XiaomiCircle("小米智能手环2代时尚版");
    }
};
// 生产华为产品簇的工厂
class HuaweiFactory : public AbstractFactory {
  public:
    ProductA *createPhone() {
        // 华为工厂肯定生产华为手机
        return new HuaweiPhone("华为荣耀7x");
    }
    ProductB *createSmartCircle() {
        // 华为工厂肯定生产华为智能手环
        return new HuaweiCircle("华为智能手环B3青春版");
    }
};

int main() {
    // 使用智能指针自动释放堆内存
    // 创建具体的工厂
    unique_ptr<AbstractFactory> f1(new XiaomiFactory);
    unique_ptr<AbstractFactory> f2(new HuaweiFactory);

    // 通过工厂方法创建手机产品
    unique_ptr<ProductA> p1(f1->createPhone());
    unique_ptr<ProductA> p2(f2->createPhone());
    p1->show();
    p2->show();

    // 通过工厂方法创建智能手环产品
    unique_ptr<ProductB> p3(f1->createSmartCircle());
    unique_ptr<ProductB> p4(f2->createSmartCircle());
    p3->show();
    p4->show();

    return 0;
}
```

#### 总结

- 简单工厂 Simple Factory: 
  - 特点：把对象的创建封装在一个接口函数里面，通过传入不同的标识，返回创建的对象客户不用自己负责new对象，不用了解对象创建的详细过程
  - 缺点：提供创建对象实例的接口函数不闭合，不能对修改关闭

- 工厂方法 Factory Method
  - 特点：Factory 基类，提供了一个纯虛函数（创建产品），定义派生类（具体产品的工厂）负责创建对应的产品，可以做到不同的产品，在不同的工厂里面创建，能够对现有工厂，以及产品的修改关闭
  - 缺点：很多产品是有关联关系的，属于一个产品簇，不应该放在不同的工厂里面去创建，这样一是不符合实际的产品对象创建逻辑，二是工厂类太多了，不好维护

- 抽象工厂 Abstract Factory
  - 特点：把有关联关系的，属于一个产品簇的所有产品创建的接口函数，放在一个抽象工厂里 AbstractFactory，派生类（具体产品的工厂）应该负责创建该产品簇里面所有的产品

- 参考：[C++设计模式 - 简单工厂，工厂方法和抽象工厂](https://blog.csdn.net/QIANGWEIYUAN/article/details/88792594)



### 3. 观察者模式

观察者模式，是一种行为性模型，又叫发布-订阅模式，定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新。比如拍卖的时候，拍卖师观察最高标价，然后通知给其他竞价者竞价。

> 行为性模型：关注系统中对象之间的相互交互，解决系统在运行时对象之间的相互通信和协作，进一步明确对象的职责。

```cpp
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
using namespace std;

// 定义监听者基类
class Listener {
  public:
    Listener(string name) : _name(name) {} // 基类构造函数
    virtual void handleMessage(int msgid) = 0; // 监听者处理消息事件的纯虚函数接口

  protected:
    string _name;
};

// 一个具体的监听者类Listener1
class Listener1 : public Listener {
  public:
    Listener1(string name) : Listener(name) {}

    void handleMessage(int msgid) { // Listener1处理自己感兴趣的事件
        cout << "listener:" << _name << " recv:" << msgid << " msg, handle it now!" << endl;
    }
};
// 一个具体的监听者类Listener2
class Listener2 : public Listener {
  public:
    Listener2(string name) : Listener(name) {}
    
    void handleMessage(int msgid) { // Listener2处理自己感兴趣的事件
        cout << "listener:" << _name << " recv:" << msgid << " msg, handle it now!" << endl;
    }
};

// 实现观察者
class Observer {
  public:
    /*
    params:
    1. Listener *pListener: 具体的监听者
    2. int msgid： 监听者感兴趣的事件
    该函数接口主要用于监听者向观察者注册感兴趣的事件
    */
    void registerListener(Listener *pListener, int msgid) { listenerMap[msgid].push_back(pListener); }
    
    /*
    params:
    1. int msgid：观察到发生的事件 id 
    该函数接口主要用于观察者观察到事件发生，并转发到对该事件感兴趣的监听者
    */
    void dispatchMessage(int msgid) {
        auto it = listenerMap.find(msgid);
        if (it != listenerMap.end()) {
            // 观察者派生事件到感兴趣的监听者，
            // 监听者通过handleMessage接口负责事件的具体处理操作
            for_each(it->second.begin(), it->second.end(), [&msgid](Listener *pListener) -> void {
                pListener->handleMessage(msgid);
            });
        }
    }

  private:
    // 存储监听者注册的感兴趣的事件
    unordered_map<int, list<Listener *>> listenerMap;
};

int main() {
    Listener *p1 = new Listener1("高海山");
    Listener *p2 = new Listener2("冯丽婷");

    Observer obser;
    // 监听者p1注册1，2，3号事件
    obser.registerListener(p1, 1);
    obser.registerListener(p1, 2);
    obser.registerListener(p1, 3);
    // 监听者p2注册1，3号事件
    obser.registerListener(p2, 1);
    obser.registerListener(p2, 3);

    // 模拟事件的发生
    int msgid;
    while(true) {
        cout << "输入事件id:";
        cin >> msgid;
        if (-1 == msgid) break;

        // 通过用户手动输入msgid模拟事件发生，此处观察者派发事件
        obser.dispatchMessage(msgid);
    }

    return 0;
}

```

#### 总结

- 上述 dispatchMessage 显然不是线程安全的，多线程环境下需要使用 shared_ptr 和 weak_ptr

- 参考：[C++设计模式 - 观察者Observer模式](https://blog.csdn.net/QIANGWEIYUAN/article/details/88745835) :fire:



---

以下属于结构型模式，关注类和对象的组合

---

### 4. 代理模式

考虑视频网站的不同视频：免费观看、VIP观看、用券观看 

考虑视频类 VideoSite，委托类 FixBugVideoSite，分别对不同的游客身份制定不同的代理类 FreeVideoSiteProxy, VIPVideoSiteProxy 等，代理类和委托类之间是组合关系（代理包含委托）

代码：todo



### 5. 装饰器模式

考虑给不同类型的车添加功能，例如有定速巡航、自动刹车、车道偏离等；现有类 Bmw, Audi, Benz 等，如果单独考虑给每个车添加不同的子类实现新功能那就太麻烦，试想想如果有100个车类那就要添加100*3个子类，此时就可以考虑装饰器模式，继承基类实现三个新功能的装饰器即可



### 6. 适配器模式

考虑电脑要投影到投影仪上需要使用不同的接口场景，例如有 VGA、HDMI 等接口

现在只有一个 VGA 接口的投影仪 TV01，但是电脑 Computer 只能使用 HDMI 接口，因此可以在 VGA 和 HDMI 之间加一个适配器类 VGAToHDMIAdapter 作为桥梁让不兼容的接口可以一起工作

代码：todo





### 参考

- [常见C++设计模式面试题和场景题](https://blog.nowcoder.net/n/85715187642d4f8c8c19161be64892da)
- [设计模式面试题（总结最全面的面试题！！！）](https://juejin.cn/post/6844904125721772039)
- [C++面试题之设计模式及设计原则](https://blog.csdn.net/QIJINGBO123/article/details/108539158)
