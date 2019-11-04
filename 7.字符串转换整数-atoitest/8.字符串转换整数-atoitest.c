/**
* @file		~/.leetcode/test.c
* @details  	
* @author  	MCD
* @date  	2019-11-04-14:05
* @par Copyright (c):
        MCD
* @version  	V1.0.0
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 201911041405 V1.0.0				MCD			create
*/

#include "common.h"

/**
* @details  	
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 201911041405 V1.0.0				MCD				create
*/
int myAtoi(char * str){
    long int result = 0;
    int line = 0;
    int i = 0;
    int flag = 1;
    int bit = 0;

    line = strlen(str);
    // print_
    mcd("str line: %d", line);

    while(str[i] == ' '){
        i++;
    }
    if(str[i] == '-'  || str[i] == '+')
    {
        if(str[i] == '-')   flag = 0;
        else flag = 1;
        // print_mcd("flag = %d", flag);
        i++;
        if(str[i] < '0' && str[i] > '9')
            return 0;
    } 
    // print_mcd("str[%d] = %c", i, str[i]);
    while(str[i] >= '0' && str[i] <= '9'){
        
        result = result * 10 + (str[i] - '0');
        
        if(result > pow(2, 31)){
            result = pow(2, 31);
            if(flag == 0){
                return result = 0 - result;
            return result;
            }
        }

        // print_mcd("str[%d] = %c, result = %ld", i, str[i], result);
        bit++;
        i++;
    }
    // print_mcd("flag = %d", flag);
    if(flag == 0){
        if(result > 2147483648)
            result = pow(2,31);
        result = 0 - result;
    }
    if(result == 2147483648)
        return result - 1;
        
    return result;
}

int main(int argc, char *argv[])
{
    print_mcd("get return value = %d", myAtoi(argv[1]));
}