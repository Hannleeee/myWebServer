#include <bits/stdc++.h>
using namespace std;

// 找到不大于target的元素的最大秩
int binarySearch(vector<int> &nums, int target) {
    int lo = 0, hi = nums.size();
    while (lo<hi) {
        int mid = lo + (hi-lo>>1);
        // 若target<nums[mid]，则进入[lo, mid), 否则进入(mid, hi)
        // 注意此时若target==nums[mid]，搜索不会终止，而是舍弃mid，进入[mid+1, hi)继续搜索
        // 所以若存在target，则最终搜索终止时mid自然为最后等于target者
        // 也即退出搜索时lo=mid+1,故应返回--lo
        // 也即最终找到的lo是第一个大于target元素！所以lo-1也就是最后一个小于等于target的元素
        target<nums[mid] ? hi = mid : lo = mid + 1;
    }
    // 若返回值为-1则说明所有元素都大于target
    return --lo;
}

// 找到大于等于target的元素的最小秩
int binarySearch1(vector<int> &nums, int target) {
    int lo = 0, hi = nums.size();
    while (lo<hi) {
        int mid = lo + (hi-lo>>1);
        // 当找到target==nums[mid]时，搜索不会终止，而是舍弃mid，进入(lo, mid)继续搜索
        // 所以若存在target，则最终搜索终止时mid自然为第一个等于target者
        target>nums[mid] ? lo = mid + 1 : hi = mid;
    }
    // 也即退出搜索时hi=mid,故应返回hi
    // 若返回值为nums.size()，则说明所有元素都小于target
    return hi;
}

int main() {
    vector<int> nums = {1,2,5,7,14};
    cout << binarySearch1(nums, 0);
}