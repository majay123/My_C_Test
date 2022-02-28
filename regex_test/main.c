/*
 *                   江城子 . 程序员之歌
 * 
 *               十年生死两茫茫，写程序，到天亮。
 *                   千行代码，Bug何处藏。
 *               纵使上线又怎样，朝令改，夕断肠。
 * 
 *               领导每天新想法，天天改，日日忙。
 *                   相顾无言，惟有泪千行。
 *               每晚灯火阑珊处，夜难寐，加班狂。
 * 
 * 
 * 
 * ************Copyright 2022 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2022-02-24 15:58:39
 * @LastEditors  : MCD
 * @LastEditTime : 2022-02-28 16:31:01
 * @FilePath     : /My_C_Test/regex_test/main.c
 * @Description  : 正则表达式测试
 * 
 * ******************************************
 */
#include "cJSON.h"
#include <regex.h>
#include <stdio.h>
// ["灯一","灯二","灯三","一孔插座","七孔插座t","七孔插座t","七孔插座t","七孔插座t","七孔插座t","七孔插座t","七孔插座t","LCR窗帘","LCR那种","开关卧室","客厅灯","房间灯1","大厅灯1","客厅灯2","客厅灯","开关1","开关_2","大门灯1","大廊灯","开关","冲冲冲","射光灯","阳台灯","厕所灯","厨房灯","背光开关","楼道灯","开关1","开关2","开关3","大灯","空调","水浸传感器"]
int main(int argc, char const *argv[])
{
    /* code */
    regex_t preg;
    regmatch_t subs[1];
    int err = 0;
    int i = 0;
    char reg_buf[] = "^[A-Z\u4e00-\u9fa5]+[A-Z0-9\u4e00-\u9fa5]$";
    char json_buf[] = "[\"灯一\",\"灯二\",\"灯三\",\"一孔插座\",\"七孔插座t\",\"七孔插座t\",\"七孔插座t\",\"七孔插座t\",\"七孔插座t\",\"七孔插座t\",\"七孔插座t\",\"LCR窗帘\",\"LCR那种\",\"开关卧室\",\"客厅灯\",\"房间灯1\",\"大厅灯1\",\"客厅灯2\",\"客厅灯\",\"开关1\",\"开关_2\",\"大门灯1\",\"大廊灯\",\"开关\",\"冲冲冲\",\"射光灯\",\"阳台灯\",\"厕所灯\",\"厨房灯\",\"背光开关\",\"楼道灯\",\"开关1\",\"开关2\",\"开关3\",\"大灯\",\"空调\",\"水浸传感器\",\"USB开关1\"]";
    // char reg_buf1[] = "/^((ht|f)tps?:\\/\\/)?[\\w-]+(\\.[\\w-]+)+:\\d{1,5}\\/?$/";
    // printf("%s\n", reg_buf);
    // 七孔插座t
    cJSON *root = cJSON_Parse(json_buf);
    if (root == NULL)
        return -1;
    int size = cJSON_GetArraySize(root);
    printf("size = %d\n", size);
    err = regcomp(&preg, reg_buf, REG_EXTENDED);
    if (err != 0) {
        printf("regcomp error %d\n");
        if(root)
            cJSON_Delete(root);
        return -1;
    }
    cJSON *item;
    for (i = 0; i < size; i++) {
        /* code */
        item = cJSON_GetArrayItem(root, i);
        // printf("item = %s\n", item->valuestring);
        err = regexec(&preg, item->valuestring, 0, NULL, 0);
        if (REG_NOMATCH == err)
            printf(" %s, no match!\n", item->valuestring);
        else
            printf("%s, match\n", item->valuestring);
    }

    regfree(&preg);
    if(root)
        cJSON_Delete(root);
    return 0;
}
