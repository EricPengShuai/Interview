#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

bool isPossible(vector<int>& nums) 
{
    unordered_map<int, int> nc, tail;
    // 遍历vector
    for(vector<int>::iterator it = nums.begin(); it != nums.end(); ++ it) {
        nc[*it] ++;
    }
    // 遍历unordered_map
    for(unordered_map<int, int>::iterator it = nc.begin(); it != nc.end(); ++ it) {
        cout << "nc.first: " << (*it).first << " nc.second: " << (*it).second << endl;
    }
    
    // LeetCode 659:https://leetcode-cn.com/problems/split-array-into-consecutive-subsequences/
    for(vector<int>::iterator it = nums.begin(); it != nums.end(); ++ it) {
        if (nc[(*it)] == 0) {
            continue;
        } 
        // 这个else if顺序不能调换
        else if (nc[(*it)] > 0 && nc[(*it)+1] > 0 && nc[(*it)+2] > 0) {
            nc[(*it)] --;
            nc[(*it)+1] --;
            nc[(*it)+2] --;
            tail[(*it)+2] ++;
        } else if (nc[(*it)] > 0 && tail[(*it)-1] > 0) {
            tail[(*it)-1] --;
            tail[(*it)] ++;  
            nc[(*it)] --;
        } else {
            return false;
        }
    }
    // 遍历unordered_map
    for(unordered_map<int, int>::iterator it = tail.begin(); it != tail.end(); ++ it) {
        cout << "tail.first: " << (*it).first << " tail.second: " << (*it).second << endl;
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
    return 0;
}

