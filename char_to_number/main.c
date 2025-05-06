/*
 * 
 * 　　┏┓　　　┏┓+ +
 * 　┏┛┻━━━┛┻┓ + +
 * 　┃　　　　　　　┃ 　
 * 　┃　　　━　　　┃ ++ + + +
 *  ████━████ ┃+
 * 　┃　　　　　　　┃ +
 * 　┃　　　┻　　　┃
 * 　┃　　　　　　　┃ + +
 * 　┗━┓　　　┏━┛
 * 　　　┃　　　┃　　　　　　　　　　　
 * 　　　┃　　　┃ + + + +
 * 　　　┃　　　┃
 * 　　　┃　　　┃ +  神兽保佑
 * 　　　┃　　　┃    代码无bug　　
 * 　　　┃　　　┃　　+　　　　　　　　　
 * 　　　┃　 　　┗━━━┓ + +
 * 　　　┃ 　　　　　　　┣┓
 * 　　　┃ 　　　　　　　┏┛
 * 　　　┗┓┓┏━┳┓┏┛ + + + +
 * 　　　　┃┫┫　┃┫┫
 * 　　　　┗┻┛　┗┻┛+ + + +
 * 
 * 
 * 
 * ************Copyright 2024 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2024-03-18 16:59:56
 * @LastEditors  : MCD
 * @LastEditTime : 2024-12-17 16:39:37
 * @FilePath     : /My_C_Test/char_to_number/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <string.h>


#define BLK_SHIFT       (9)
#define BLK_SIZE        (1 << BLK_SHIFT)

#define ROUND(x) ((x) >= 0 ? (unsigned char)((x) + 0.5) : (unsigned char)((x) - 0.5))
#define DBVAL(val)          (double)((100.0 / 40) * (val))
#define TYVAL(val)          (double)((40.0 / 100) * (val))

/* from lib/kstrtox.c */
static const char *_parse_integer_fixup_radix(const char *s, uint *basep)
{
    /* Look for a 0x prefix */
    if (s[0] == '0') {
        int ch = tolower(s[1]);

        if (ch == 'x') {
            *basep = 16;
            s += 2;
        }
        else if (!*basep) {
            /* Only select octal if we don't have a base */
            *basep = 8;
        }
    }

    /* Use decimal by default */
    if (!*basep)
        *basep = 10;

    return s;
}

static uint decode_digit(int ch)
{
    if (!isxdigit(ch))
        return 256;

    ch = tolower(ch);

    return ch <= '9' ? ch - '0' : ch - 'a' + 0xa;
}

ulong simple_strtoul(const char *cp, char **endp, uint base)
{
    ulong result = 0;
    uint value;

    cp = _parse_integer_fixup_radix(cp, &base);

    while (value = decode_digit(*cp), value < base) {
        result = result * base + value;
        cp++;
    }

    if (endp)
        *endp = (char *)cp;

    return result;
}

static uint8_t test_num = 8;

static int _test(uint8_t *status) {
    int ret = 0;
    *status = test_num;

    return ret;
}


// // 判断字符串是否为合法数字
// int is_valid_number(const char *str)
// {
//     // 检查空字符串
//     if (str == NULL || *str == '\0') {
//         return 0;
//     }

//     // 允许开头的正负号
//     if (*str == '-' || *str == '+') {
//         str++;
//     }

//     // 检查剩余字符是否都是数字
//     while (*str) {
//         if (!isdigit(*str)) {
//             return 0;
//         }
//         str++;
//     }

//     return 1;
// }

struct utf8_table
{
    int     cmask;
    int     cval;
    int     shift;
    long    lmask;
    long    lval;
};

static struct utf8_table utf8_table[] =
{
    {0x80,  0x00,   0*6,    0x7F,           0,         /* 1 byte sequence */},
    {0xE0,  0xC0,   1*6,    0x7FF,          0x80,      /* 2 byte sequence */},
    {0xF0,  0xE0,   2*6,    0xFFFF,         0x800,     /* 3 byte sequence */},
    {0xF8,  0xF0,   3*6,    0x1FFFFF,       0x10000,   /* 4 byte sequence */},
    {0xFC,  0xF8,   4*6,    0x3FFFFFF,      0x200000,  /* 5 byte sequence */},
    {0xFE,  0xFC,   5*6,    0x7FFFFFFF,     0x4000000, /* 6 byte sequence */},
    {0,     0,      0,      0,              0,/* end of table    */}
};

static int utf8_mbtowc(unsigned short *p, const unsigned char *s, int n)
{
    long l;
    int c0, c, nc;
    struct utf8_table *t;

    nc = 0;
    c0 = (int) (*s);
    l = c0;
    for (t = utf8_table; 0 != t->cmask; t++)
    {
        nc++;
        if ((c0 & t->cmask) == t->cval)
        {
            l &= t->lmask;
            if (l < t->lval)
            {
                return -1;
            }
            *p = (unsigned short) l;
            return nc;
        }
        if (n <= nc)
        {
            return -1;
        }
        s++;
        c = (*s ^ 0x80) & 0xFF;
        if (0 != (c & 0xC0))
        {
            return -1;
        }
        l = (l << 6) | c;
    }
    return -1;
}

int utf8_mbstowcs(unsigned short *pwcs, const unsigned char *s, int n)
{
    unsigned short *op;
    const unsigned char *ip;
    int size;

    op = pwcs;
    ip = s;
    while ((0 != (*ip)) && (n > 0))
    {
        if (0 != ((*ip) & 0x80))
        {
            size = utf8_mbtowc(op, ip, n);
            if (size == -1)
            {
                /* Ignore character and move on */
                ip++;
                n--;
            }
            else
            {
                op++;
                ip += size;
                n -= size;
            }
        }
        else
        {
            *op = *ip;
            op++;
            ip++;
            n--;
        }
    }
    return (op - pwcs);
}


// 判断字符串是否为合法数字
int is_valid_number(unsigned char *str)
{
    int len = 0;
    // 检查空字符串
    if (str == NULL || *str == '\0') {
        return 0;
    }

#if 0
    // 允许开头的正负号
    if (*str == '-' || *str == '+') {
        str++;
    }
#endif

    // 检查剩余字符是否都是数字
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
        len++;
    }

    return len;
}

int main()
{
    // unsigned long size, blk_size;

    // size = simple_strtoul("ab946c", NULL, 16);
    // printf("size = %ld\n", size);
    // blk_size = (size >> BLK_SHIFT) + !!(size & (BLK_SIZE - 1));
    // printf("blk_size = %ld\n", blk_size);

    uint8_t status = 0;
    _test(&status);
    char buf[10] = "jsafdas;";
    strlen(buf);
    printf("status = %d, %d\n", status, strlen(buf));

    unsigned short *psrcStr = "222";
    unsigned short *pdestStr = "1112a";
    char *name = "123";
    int32_t len;
    int32_t unicode_len = 0;
    uint16_t pwcs[512];

    
    // printf("%d, pdestStr = %s\n", is_valid_number((const char *)pdestStr), (const char *)pdestStr);

    len = strlen(name) + 1;
    unicode_len = utf8_mbstowcs(pwcs, (unsigned char*)name, len);
    if(unicode_len <= 0)
    {
        printf("convert to unicode fail, %s\n", name);
        return -1;
    }
    printf("unsigned short name[%d] = { ", unicode_len);
    for(unsigned int i = 0; i < unicode_len; i++)
    {
        printf("0x%04x, ", pwcs[i]);
    }
    printf("}\n");

    printf("unicode_len = %d, %s, %d\n", unicode_len, pwcs, strlen(pwcs));    
    pwcs[unicode_len] = 0;

    printf(" %d, %d\n", is_valid_number(psrcStr), is_valid_number(pdestStr));

    printf("change val: %d\n", ROUND(TYVAL(89)));

    char *str = "test1";
    switch (str) {
        case "test1" : {
            printf("test1\n");
            break;
        }
    }


    return 0;
}