## 构造数据结构以及处理输入技巧

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



#### 3.2 getline

**#include <string\>**

```cpp
// ss 是输入流，item 是读取的字符串，delim 是分隔符(默认是'\n')
string = "1,2,3,4,5,6,7";
vector<int> output;
stringstream ss;
ss.str(input);
string item;
char delim = ',';
while (getline(ss, item, delim)) {
  output.push_back(stoi(item));
}
```



**#include <istream\>**

```cpp
istream& getline (char* s, streamsize n );
istream& getline (char* s, streamsize n, char delim );

// istream::getline example
#include <iostream>     // std::cin, std::cout
using namespace std;

int main () {
  char name[256], title[256];

  cout << "Please, enter your name: ";
  cin.getline (name,256);

  cout << "Please, enter your favourite movie: ";
  cin.getline (title,256);

  cout << name << "'s favourite movie is " << title;
  return 0;
}
```

