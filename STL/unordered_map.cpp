#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

bool isPossible(vector<int>& nums) 
{
    unordered_map<int, int> nc, tail;
    // 遍历vector
    for(int & num : nums) {
        nc[num] ++;
    }
    // 遍历unordered_map
    for(auto & it : nc) {
        cout << "nc.first: " << it.first << " nc.second: " << it.second << endl;
    }
    
    // LeetCode 659:https://leetcode-cn.com/problems/split-array-into-consecutive-subsequences/
    for(int & num : nums) {
        if (nc[num] == 0) {
            continue;
        } 
        // 这个else if顺序不能调换
        else if (nc[num] > 0 && nc[num+1] > 0 && nc[num+2] > 0) {
            nc[num] --;
            nc[num+1] --;
            nc[num+2] --;
            tail[num+2] ++;
        } else if (nc[num] > 0 && tail[num-1] > 0) {
            tail[num-1] --;
            tail[num] ++;
            nc[num] --;
        } else {
            return false;
        }
    }
    // 遍历unordered_map
    for(auto & it : tail) {
        cout << "tail.first: " << it.first << " tail.second: " << it.second << endl;
    }
    return true;
}

int main()
{
    int a[5] = {1, 2, 3, 4, 5};
	vector<int> nums(a, a + 5);
    // C++11新方式
    for(auto num: nums) {
        cout << num << ' ';
    }
    cout << endl;
    cout << isPossible(nums);
    cout << endl;

    unordered_map<char, int> mp;
    for(int i = 0; i < 5; ++ i) {
        mp['a' + i] = i;
    }
    for (auto i : mp) {
        cout << i.first << "->" << i.second << ' ';
    }
    cout << endl;

    // unordered_map查找某个键或者同级某个键
    cout << "find('a')  " << (*mp.find('a')).first << "->" << (*mp.find('a')).second << endl;
    cout << "count('a') " << mp.count('a') << endl;


    // 初始化unordered_map
    unordered_map<int, int> mpa;
    cout << "test: " << mpa[0] << endl;
    return 0;
}

