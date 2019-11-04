/**
* @file		~/.leetcode/test.c
* @details  	
* @author  	MCD
* @date  	2019-11-04-09:00
* @par Copyright (c):
        MCD
* @version  	V1.0.0
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 201911040900 V1.0.0				MCD			create
*/

#include "common.h"

/**
* @details  	int strStr(char * haystack, char * needle)
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 201911040901 V1.0.0				MCD				create
*/  
int strStr(char * haystack, char * needle)
{
    int line_h, line_n;

    if(needle == NULL)  return 0;

    line_h =strlen(haystack);
    line_n =strlen(needle);
    if(line_n == 0) return 0;
    print_mcd("line_h = %d, line_n = %d", line_h, line_n);
    for (int i = 0; i < line_h; i++)
    {
        int j = 0;
        while(haystack[i] == needle[j] && j < line_n){
            print_mcd("haystack[%d] = %c, needle[%d] = %c", i, haystack[i], j, needle[j]);
            i++;
            j++;
        }
        if(j == line_n) return i - line_n;
        i = i - j;
    }
    return -1;    
}

int main(int argc, char *argv[])
{
    int count = 0;

    print_mcd("get count = %d", strStr(argv[1], argv[2]));
}