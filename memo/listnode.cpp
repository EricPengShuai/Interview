#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

// 定义链表
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


// 迭代法合并链表
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* preHead = new ListNode(-1);
        ListNode* cur = nullptr;
        ListNode* pre = preHead;
        pre->next = cur;

        while(l1 && l2) {
            if (l1->val <= l2->val) {
                pre->next = l1;
                pre = pre->next;
                l1 = l1->next;
            } else {
                pre->next = l2;
                pre = pre->next;
                l2 = l2->next;
            }
        }
        
        pre->next = l1 == nullptr ? l2 : l1;
        return preHead->next;
    }
};


// 去掉左边 white-space：https://cplusplus.com/reference/cctype/isspace/?kw=isspace
void trimLeftTrailingSpaces(string &input) {
    input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
        return !isspace(ch);
    }));
}

// base 从其 reverse_iterator还原基础迭代器，因为 earse 参数只能是基础迭代器
void trimRightTrailingSpaces(string &input) {
    input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), input.end());
}

// 字符串输入转 vector：[1,2,3,4,5]
vector<int> stringToIntegerVector(string input) {
    vector<int> output;
    trimLeftTrailingSpaces(input);
    trimRightTrailingSpaces(input);
    input = input.substr(1, input.length() - 2);
    stringstream ss;
    ss.str(input);
    string item;
    char delim = ',';
    while (getline(ss, item, delim)) {
        output.push_back(stoi(item));
    }
    return output;
}


// 注意构造链表的方式
ListNode* stringToListNode(string input) {
    // Generate list from the input
    vector<int> list = stringToIntegerVector(input);

    // Now convert that list into linked list
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


// 链表转字符串
string listNodeToString(ListNode* node) {
    if (node == nullptr) {
        return "[]";
    }

    string result;
    while (node) {
        result += to_string(node->val) + ", ";
        node = node->next;
    }
    return "[" + result.substr(0, result.length() - 2) + "]";
}


int main() {
    string line;

    // string 文件中的 getline(cin, line, delim='\n')
    while (getline(cin, line)) {
        if(line == "quit") {
            cout << "bye~~\n";
            return 0;
        }
        ListNode* list1 = stringToListNode(line);
        getline(cin, line);
        ListNode* list2 = stringToListNode(line);
        
        ListNode* ret = Solution().mergeTwoLists(list1, list2);

        string out = listNodeToString(ret);
        cout << out << endl;
    }
    return 0;
}