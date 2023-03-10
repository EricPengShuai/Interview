## 应试技巧

> 有的公司面试你需要自己处理输入输出，长时间刷力扣容易忽略这点，在牛客网上面试往往需要自己处理输入输出，这个时候需要注意常见结构体的构造以及相关输入处理技巧



### 1. 链表

#### 1.1 结构体

```cpp
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
```

  

#### 1.2 构造链表

```cpp
ListNode* stringToListNode(vector<int> list) {
    // 防止输入为空
    ListNode* dummyRoot = new ListNode(0);
    ListNode* ptr = dummyRoot;
    for(int item : list) {
        ptr->next = new ListNode(item);
        ptr = ptr->next;
    }
    ptr = dummyRoot->next;
    delete dummyRoot;
    return ptr;
}
```



参考代码：[listnode.cpp](./listnode.cpp)



### 2. 二叉树

#### 2.1 结构体

```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(): val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x): val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right): val(x), left(left), right(right) {}
};
```

  

#### 2.2 构造二叉树

力扣中一般是层序遍历形式输入的，所以通过引入队列层序遍历构造二叉树

```cpp
// 输入 [2,1,3,null,4,null,7]
TreeNode* stringToTreeNode(string input) {
    input = input.substr(1, input.length() - 2);
    if (!input.size()) {
        return nullptr;
    }

    string item;
    stringstream ss;
    ss.str(input);

    // 输入是层序遍历形式
    getline(ss, item, ',');
    TreeNode* root = new TreeNode(stoi(item));
    queue<TreeNode*> nodeQueue;
    nodeQueue.push(root);

    while (true) {
        TreeNode* node = nodeQueue.front();
        nodeQueue.pop();

        if (!getline(ss, item, ',')) {
            break;
        }

        trimLeftTrailingSpaces(item);
        if (item != "null") {
            int leftNumber = stoi(item);
            node->left = new TreeNode(leftNumber);
            nodeQueue.push(node->left);
        }

        if (!getline(ss, item, ',')) {
            break;
        }

        trimLeftTrailingSpaces(item);
        if (item != "null") {
            int rightNumber = stoi(item);
            node->right = new TreeNode(rightNumber);
            nodeQueue.push(node->right);
        }
    }
    return root;
}
```



参考代码：[treenode.cpp](./treenode.cpp)



### 3. 输入处理技巧

#### 3.1 处理左右空白字符串

```cpp
// 去掉左空白字符串
void trimLeftTrailingSpaces(string &input) {
    input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
        return !isspace(ch);
    }));
}

// base 从其 reverse_iterator还原基础迭代器，因为 earse 参数只能是基础迭代器
// 去掉右空白字符串
void trimRightTrailingSpaces(string &input) {
    input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), input.end());
}
```



#### 3.2 输入数组中带有括号

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <cstring> // char *strtok(char *str, const char *delim);
#include <sstream>
using namespace std;

int main() {
    // 处理没有给出矩阵的行列数，例如输入为：
    // 3 2 3
    // 1 6 5
    // 7 8 9
    vector<vector<int>> arr;
    string input;
    while (getline(cin, input)) {
        if (input.size() > 0) {
            stringstream stringin(input);
            int num;
            vector<int> a;
            while (stringin >> num) {
                a.push_back(num);
            }
            arr.push_back(a);
        } else {
            break;
        }
    }
    
    // 输入中带括号，例如输入为：
    // [[3, 2, 3],
    // [1, 6, 5],
    // [7, 8, 9]]
    vector<vector<int>> arr;
    string input;
    char *tok;
    while (getline(cin, input)) {
        // 其实这里如果不是输入文件，还需要 break，但是牛客上都是文件形式有 EOF
        if (input.size() > 0) {
            vector<int> a;
            tok = strtok((char *)input.c_str(), " ,[]");
            while (tok != NULL) {
                a.push_back(stoi(tok));
                tok = strtok(NULL, " ,[]");
            }
            arr.push_back(a);
        }
    }
    return 0;
}
```

**注意 strtok 的用法**

> 在第一次调用时，strtok函数需要将要分解的字符串指针作为第一个参数传递进去。在这之后，每次调用strtok函数时，第一个参数应该为NULL，以便函数知道应该从上一次调用的位置继续分解字符串。这个位置会被记录下来，并存储在内部的静态变量中。
>
> 传递NULL的作用是告诉strtok函数继续从上一次的位置继续分解字符串。因此，如果您在同一个字符串上多次调用strtok函数，则它将继续从上次分解的位置开始进行分解。这是因为strtok函数使用一个静态变量来存储上次分解的位置，而这个静态变量会在每次调用strtok函数时更新。
>
> 需要注意的是，如果你想使用strtok函数分解多个字符串，你需要为每个新字符串调用一次strtok函数

```cpp
#include <cstring>
#include <iostream>

int main()
{
    char str1[] = "Hello world";
    char str2[] = "How are you?";
    char *token;

    // 分解第一个字符串
    token = std::strtok(str1, " ");
    while (token != NULL) {
        std::cout << token << '\n';
        token = std::strtok(NULL, " ");
    }

    // 分解第二个字符串
    token = std::strtok(str2, " ");
    while (token != NULL) {
        std::cout << token << '\n';
        token = std::strtok(NULL, " ");
    }

    return 0;
}
```





### 4. TLE

- 往往就是空间换时间：根据题意选择合适数据结构
  - 大顶堆：`priority_queue<int, vector<int>, lese<int>>`，堆顶是最大元素，插入元素 $O(\log N)$
  - 红黑数：`set<int>` `map<int, int>`，默认是按照 key 升序（set 中是 value），插入删除 $O(\log N)$
  - 哈希表：`unordered_set<int>` `unordered_map<int, int>`，查找比较快

- 有些牛客上的题目卡输入输出：`cout << x << endl;` 如果输出次数比较多，由于 endl 会刷新输出流缓冲区，多行输出会导致刷新缓冲区过于频繁，非常耗时间，可以使用 '\n' 代替 endl，例如：[公司食堂](https://www.nowcoder.com/questionTerminal/601815bea5544f389bcd20fb5ebca6a8)



