/*
 * @lc app=leetcode.cn id=633 lang=c
 *
 * [633] 平方数之和
 */

// @lc code=start
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool judgeSquareSum(int c)
{
    int i = 0;

    if(c == 0 || c == 1)
        return true;

    for (i = 0; i <= sqrt(c); i++)
    {
        int n = (int)sqrt(c - (i * i));
        if (c == (n * n + i * i))
            return true;
    }

    return false;
}

// @lc code=end
