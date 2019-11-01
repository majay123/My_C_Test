/**
* @file		~/.leetcode/279.完全平方数test.c
* @details  	
* @author  	MCD
* @date  	2019-10-31-13:35
* @par Copyright (c):
        MCD
* @version  	V1.0.0
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 201910311335 V1.0.0				MCD			create
*/

#include "common.h"

/**
* @details      int numSquares(int n)	
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 201910311335 V1.0.0				MCD				create
*/
#if 0
int numSquares(int n)
{
    while (n % 4 == 0)
        n /= 4;
    if (n % 8 == 7)
        return 4;
    if(n == 1)  return 1;
    for (int i = 0; i * i <= n; ++i)
    {
        // print_mcd("i = %d", i);
        int a = (int)sqrt(n - i * i);
        if (a * a + i * i == n)
        {
            if (a == 0)
                return 1;
            return 2;
        }
    }
    return 3;
}
#else
int numSquares(int n)
{
    int dp[] = {0};
    
    for (int i = 0; i <=n; i++){
        dp[i] = i;
        int top = (int)sqrt(i);
        for (int j = 1; j <= top; j++){
            dp[i] = min(dp[i], dp[i - j * j] + 1);
        }
    }
    return dp[n];
}
#endif
int main(int argc, char *argv[])
{
    print_mcd("return count = %d", numSquares(1));
    return numSquares(1);
}