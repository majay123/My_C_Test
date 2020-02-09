/*
 * @lc app=leetcode.cn id=345 lang=c
 *
 * [345] 反转字符串中的元音字母
 */

// @lc code=start
#include <string.h>
#include <stdio.h>

static int _check_char(char *s, int strsize, char c)
{
    int i = 0;

    if (s == NULL)
        return 0;

    for (i = 0; i < strsize; i++)
    {
        if (s[i] == c)
            return 1;
    }

    return 0;
}

char *reverseVowels(char *s)
{
    const char array[10] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
    char *tmep_s = s;
    int len = strlen(s);
    int i = 0;

    if (s == NULL)
        return NULL;

    for (i = 0; i < len; )
    {
        char c_a = s[i];
        char c_b = s[len];
        char c_temp = 0;

        if(!_check_char((char *)array, 10, c_a))
            s[i++] = c_a;
        else if(!_check_char((char *)array, 10, c_b))
            s[len--] = c_b;
        else
        {
            s[i++] = c_b;
            s[len--] = c_a;
        }     
    }

    return s;
}

// @lc code=end
