/*
 * @lc app=leetcode.cn id=680 lang=c
 *
 * [680] 验证回文字符串 Ⅱ
 */

// @lc code=start
#include <stdio.h>
#include <string.h>

static bool _check_char(char *s, int start, int end)
{
    while(start < end){
        if(s[start++] != s[end--])
            return false;
    }
    return true;
}

bool validPalindrome(char *s)
{
    int s_size = strlen(s) - 1;
    int i = 0;
    int count = 0;

    if (s_size > 50000)
        return false;

    for(i = 0; i < s_size; i++, s_size--){
        if(s[i] != s[s_size]){
            return _check_char(s, i + 1, s_size) || _check_char(s, i, s_size - 1);
        }
    }
    return true;

   
}

// @lc code=end
