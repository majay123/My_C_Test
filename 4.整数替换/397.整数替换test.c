/**
* @file		~/.leetcode/397.整数替换test.c
* @details  	
* @author  	MCD
* @date  	2019-10-30-17:02
* @par Copyright (c):
        MCD
* @version  	V1.0.0
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 201910301702 V1.0.0				MCD			create
*/

#include "common.h"


#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
/**
* @details  	int integerReplacement(int n)
* @param[in]	n   
* @param[out]	count   
* @retval  		
* @brief		整数替换
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 201910301702 V1.0.0				MCD				create
*/

int integerReplacement(int n)
{
    int count = 0;
    long int tmp = n;

    if(tmp == 1 || tmp <= 0)    return 0;
    printf("%ld", tmp);
    while (tmp  != 1)
    {
        
        while((tmp % 2 == 0) && (tmp > 1)){
            tmp = tmp / 2;
            count++;
            printf("->%ld", tmp);
        }
        
            
        while((tmp % 2 == 1) && (tmp > 1)){
            if((tmp % 4) == 3)
                tmp = (tmp + 1);
            else if((tmp % 4) == 1)
                tmp = (tmp - 1);           
            count++;
            printf("->%ld", tmp); 
        }
           
    } 
    printf("\n");
    return count;
}

int main(int argc, char *argv[])
{
    int count = 0;
    int test_n = 2147483647;

    count = integerReplacement(test_n);
    print_mcd("count = %d\n", count);
}