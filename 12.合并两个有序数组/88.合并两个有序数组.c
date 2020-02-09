/*
 * @lc app=leetcode.cn id=88 lang=c
 *
 * [88] 合并两个有序数组
 */

// @lc code=start
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n){
   
    // if(m == 0){
    //     for(int i = 0; i < n; i++)
    //         nums1[i] = nums2[i];
    // }
    while(m && n){
        if(nums1[m-1] > nums2[n-1]){
            nums1[n + m -1] = nums1[m-1];
            m--;
        }
        else{
            nums1[n + m -1] = nums2[n - 1];
            n--;
        }
    }
    while(n){
        nums1[n-1] = nums2[n-1];
        n--;
    }


}

// @lc code=end

