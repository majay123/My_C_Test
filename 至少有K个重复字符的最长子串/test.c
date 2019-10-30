#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PRINT_MCD_ENABLE 1

#if PRINT_MCD_ENABLE == 1
#define print_mcd(format, arg...)                                                          \
    do                                                                                     \
    {                                                                                      \
        printf("\033[31m[-mcd-]\033[0m:%s,%d--- " format "\n", __FILE__, __LINE__, ##arg); \
    } while (0)
#else
#define print_zizy(format, arg...) \
    do                             \
    {                              \
    } while (0)
#endif

#define max(a, b) (((a) > (b)) ? (a) : (b))

int count_func(char *s, int k, int start, int end)
{
    int asc[26] = {0};
    int i = 0;

    if ((end - start + 1) < k)
        return 0;

    //统计字母出现的次数
    for (i = start; i <= end; i++)
        asc[s[i] - 'a']++;

    //分割数组
    while ((end - start + 1) >= k && asc[s[start] - 'a'] < k)
        ++start;
    while ((end - start + 1) >= k && asc[s[end] - 'a'] < k)
        --end;

    print_mcd("start = %d, end = %d", start, end);
    if ((end - start + 1) < k)
        return 0;
    for (i = start; i <= end; ++i){
        if(asc[s[i] - 'a'] < k){
            print_mcd("start = %d, end = %d", start, end);
            return max(count_func(s, k, start, i - 1), count_func(s, k, i + 1,end));
        }
            
    }
    return end - start + 1;
}

int longestSubstring(char *s, int k)
{
    int count = 0;
    int line = 0;

    line = strlen(s);
    print_mcd("s line = %d", line);
    if ((line < k) || (line == 0))
        return 0;

    return count_func(s, k, 0, line - 1);
}

int main(int argc, char *argv[])
{
    int count = 0;
    char test_string[] = "bababababababababababababababababaabababababaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbcbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    count = longestSubstring(test_string, 30);
    print_mcd("out count = %d", count);
}