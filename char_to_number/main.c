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
 * @LastEditTime : 2024-06-07 10:23:04
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

    return 0;
}