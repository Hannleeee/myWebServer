#include <bits/stdc++.h>
using namespace std;

// 找到不大于target的元素的最大秩
int bSearch(vector<int> &nums, int target) {
    int lo = 0, hi = nums.size();
    while (lo<hi) {
        int mid = lo + (hi-lo>>1);
        // 若target<nums[mid]，则进入[lo, mid), 否则进入(mid, hi)
        // 注意此时若target==nums[mid]，搜索不会终止，而是舍弃mid，进入(mid, hi)继续搜索
        // 导致最终找到的lo是第一个大于target元素！所以lo-1也就是最后一个小于等于target的元素
        
    }
    // 若返回值为-1则说明所有元素都大于target
    return --lo;
}

// 找到等于target的元素的最小秩
int bSearch(vector<int> &nums, int target) {
    int lo = 0, hi = nums.size();
    while (lo<hi) {
        int mid = lo + (hi-lo>>1);
        target>nums[mid] ? lo = mid+1 : hi = mid;
    }
}