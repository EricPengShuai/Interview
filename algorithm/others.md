## 一些小技巧

> 记录一些题目中的小算法，例如判断素数、质因数分解等



### 1. 判断素数

素数也叫质数，大于 1 的整数中，只能被 1 和其自身整除的数称为素数；1 既不是素组也不是合数

```cpp
bool isPrime(int x) {
    if (x == 1) return 
    for (int i = 2; i * i <= x; ++ i) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}
```



### 2. 质因数分解

质因数分解就是将一个数分解成各个质数因数，例如 12 = 2 * 2 * 3 ，具体思路为

1. p 从 2 遍历到 x，先找到 x 的最小质因子 p
2. 当 x == p 时，说明已经找完所有的质因子
3. 当 x > p 时，x /= p，继续从 (1) 开始遍历

```cpp
// x 是待分解的数，分解出来的质因数存储在 primes 中
void getPrimeFactor(long x, vector<long>& primes) {
    long p = 2;
    while (x != 1) {
        // p 一定是第一个可以整除 x 的最小质因数
        // 这里 p 一定是质数，没必要再次判断
        for (p = 2; p <= x; ++ p) {
            if (x % p == 0) {
                break;
            }
        }
        primes.push_back(p);
        x /= p;
    }
}

void getPrimeFactor(long x, vector<long>& primes) {
    long p = 2;
    for (p = 2; p <= x; ++ p) {
        // 找到 x 的一个质因子 p 之后就不断的除以 p，将 x 中所有的 p 全部找出
        while (x % p == 0) {
            primes.push_back(p);
            if (x == p)  break;
            x /= p;
        }
    }
}
```

**以上的方法得到的质因数中会有重复的，如果需要得到不重复的，需要改一下代码：**

```cpp
void getPrimeFactor(long x, vector<long>& primes) {
    long p = 2;
    // 假设，原数已经找到了质因子（从小到大排列，不包含重复值）p1,p2...,pt
    // 此时，x = x/(p1*p2*...*pt);
    // 在 [pt+1, x] 的范围上，找 x 的最小质因数
    while (p <= x) {
        if (x % p == 0 && isPrime(p)) {
            primes.push_back(p);
            if (x % p == 0) {
                return;
            }
            x /= p;
        }
        p += 1;
    }
}
```

**例题：**

[2507. 使用质因数之和替换后可以取到的最小值](https://leetcode.cn/problems/smallest-value-after-replacing-with-sum-of-prime-factors/)

- 参考灵神的题解，for 循环可以直接使用 `i * i < n` 判断，但是要注意推出之后和 1 判断一下

- 没必要判断是否变小，因为 $2+\frac{n}{2} \le n,(n \ge 4)$ 是一直变小的，退出条件就是 ans == n，[0x3F 暴力](https://leetcode.cn/problems/smallest-value-after-replacing-with-sum-of-prime-factors/solution/bao-li-by-endlesscheng-xh0b/)

  

### 3. 最大公约数和最小公倍数

**最大公约数（gcd）**：greatest common divisor，C++ `<numeric>`中有具体的库函数 `std::gcd`，如果自己实现可以这样写：

```cpp
int gcd(int a,int b) {
    int r;
    while(b > 0) {
        r  = a % b;
        a = b;
        b = r;
    }
    return a;
}

// 递归写法，出口条件就是 b 是否为 0
int gcd(int a,int b) {
    return b>0 ? gcd(b, a%b) : a;
}
```



**最小公倍数（lcm）**：lowest common multiple，C++ `<numeric>`中有具体的库函数 `std::lcm`，如果自己实现可以这样写：

```cpp
// lcm(a, b) = a * b / gcd(a, b)
int lcm(int a,int b){
	return a/gcd(a,b)*b;	// 注意先除可以防溢出
}
```





### 参考

- [C++实现 质因数分解、最大公约数、最小公倍数、求所有因数](https://blog.csdn.net/mumu1408/article/details/108557031)

- [gcd函数和lcm函数（c/c++）小染大大的博客](https://blog.csdn.net/m0_50176535/article/details/115129689)


