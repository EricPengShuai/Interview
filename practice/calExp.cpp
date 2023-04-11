#include <iostream>
#include <stack>
using namespace std;

// 判断字符是否为运算符（包括加减乘除）
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// 判断运算符的优先级
int priority(char c) {
    if (c == '+' || c == '-') {
        return 1;
    } else if (c == '*' || c == '/') {
        return 2;
    } else {
        return 3;   // 括号优先级最大
    }
}

string reversePolarExp(string &s) {
    string res;
    stack<char> st; // 存放操作符

    for(auto& c: s) {
        if (c == ' ') {
            continue;
        } else if (isalnum(c)) {
            res += c;   // 操作数直接输出
        } else {
            // 1. 栈为空或者左括号直接入栈
            if (st.empty() || c == '(') {
                st.push(c);
                continue;
            }

            // 3. 只有遇到右括号才弹左括号
            if (c == ')') {
                while (st.top() != '(') {
                    res += st.top();
                    st.pop();
                }
                st.pop();   // 弹出左括号
                continue;
            }

            // 2. 否则，弹出比 c 优先级大于或者等于的操作符，除了左括号
            while (!st.empty() && st.top() != '(' && priority(c) <= priority(st.top())) {
                res += st.top();
                st.pop();
            }
            st.push(c);
        }
    }
    while(!st.empty()) {    // 4. 最后弹出剩余的操作符
        res += st.top();
        st.pop();
    }
    return res;
}

// 计算逆波兰表达式的值
float calculateRPE(const string& rpn) {
    stack<float> operands; // 操作数栈

    for (char c : rpn) {
        if (isdigit(c)) {
            // 如果是数字，则转换为float并入栈
            operands.push(static_cast<float>(c - '0'));
        } else if (isOperator(c)) {
            // 如果是运算符，则从栈中取出操作数进行计算，并将结果入栈
            float operand2 = operands.top();
            operands.pop();
            float operand1 = operands.top();
            operands.pop();

            switch (c) {
                case '+':
                    operands.push(operand1 + operand2);
                    break;
                case '-':
                    operands.push(operand1 - operand2);
                    break;
                case '*':
                    operands.push(operand1 * operand2);
                    break;
                case '/':
                    operands.push(operand1 / operand2);
                    break;
                default:
                    break;
            }
        }
    }

    // 返回最终的计算结果
    return operands.top();
}

int main()
{
    string s("1+2*3+(1*2+3)*4");
    string rpe = reversePolarExp(s);
    cout << rpe << endl;
    cout << calculateRPE(rpe) << endl;
    return 0;
}