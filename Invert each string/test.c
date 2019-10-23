#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * strturn(char *str1)
{
    char *tmp = str1;
    char *str2 = str1 + strlen(str1) - 1;
    char one_str[]= {0};
    char tmpchar;
    
    while(tmp < str2){
        tmpchar = *tmp;
        *tmp++ = *str2;
        *str2-- = tmpchar;
    }
    return str1;
}

void get_str_nums(char *str)
{
    int len = strlen(str);
    int count = 0;
    char tmp[256] = {0};
    char *str_s = str;
    int tmp_len = 0;

    if(str == NULL)
        return ;
    for (int i = 0; i < len + 1; i++){   
        if(str[i] == ' ' || str[i] == '\0'){
            memset(tmp, 0, 256);
            memcpy(tmp, str + tmp_len, i - tmp_len);
            char * get_str = strturn(tmp);
            printf("%s ", get_str); 
            // if(str[i] == ' ')
            //     printf(" ");
            // printf("tmp = %s\n", tmp);
            tmp_len = i + 1;
        }    
    }
    // return NULL;
}

int main(void)
{
    char str[] = {0};
    char *get_str;
    int num = 0; 

    while(scanf("%s", str) != EOF)
    {
        get_str_nums(str);
    }
}