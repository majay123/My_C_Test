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
 * ************Copyright 2023 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2023-03-14 16:33:07
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-20 16:53:06
 * @FilePath     : /My_C_Test/Hope_Work/leshi/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ls_bt_mesh.h"
#include "ls_bt_cmd_dispatch.h"
#include "cJSON.h"
#include "common.h"

#if 0
static void _ls_back_cmd_group_delete_sub_devices_status_string(char *group, char *cid, int rest)
{
    switch (rest) {
        case LS_GROUP_DEL_SUB_DEVICE_SUCCESS:
            print_mcd("group [%s], delete sub device [%s] success!!", group, cid);
            break;
        case LS_GROUP_DEL_SUB_DEVICE_OUT_UPPER_LIMIT:
            print_mcd("group [%s], delete sub device [%s] exceed the upper limit of the number of groups!!", group, cid);
            break;
        case LS_GROUP_DEL_SUB_DEVICE_TIMEOUT:
            print_mcd("group [%s], delete sub device [%s] timeout!!", group, cid);
            break;
        case LS_GROUP_DEL_SUB_DEVICE_SET_OUT_OF_RANGE:
            print_mcd("group [%s], delete sub device [%s] setting value out of range!!", group, cid);
            break;
        case LS_GROUP_DEL_SUB_DEVICE_WRITE_FILE_ERR:
            print_mcd("group [%s], delete sub device [%s] written file error!!", group, cid);
            break;
        case LS_GROUP_DEL_SUB_DEVICE_OTHER_ERR:
            print_mcd("group [%s], delete sub device [%s] other errors!!", group, cid);
            break;

        default:
            break;
    }
}

static void _test_json(void)
{
    char *data = "{\"gid\":\"01\",\"cids\":[\"b223f59481\", \"baa3219481\"],\"rets\":[0, 1]}";

    cJSON *root = NULL;
    int i = 0;

    REQUIRE(NULL == data, Error);

    root = REQ_JSON_PARSE((const char *)data, root, Error);

    /* Processe cjson information */
    cJSON *gid = REQ_JSON_OBJ(root, gid, Error);
    cJSON *cids = REQ_JSON_OBJ(root, cids, Error);
    cJSON *rets = REQ_JSON_OBJ(root, rets, Error);

    if (gid->valuestring) {
        int csize = cJSON_GetArraySize(cids);
        int rsize = cJSON_GetArraySize(rets);
        if (csize == rsize) {
            for (i = 0; i < csize; i++) {
                cJSON *cid_js = cJSON_GetArrayItem(cids, i);
                cJSON *ret_js = cJSON_GetArrayItem(rets, i);
                _ls_back_cmd_group_delete_sub_devices_status_string(gid->valuestring, cid_js->valuestring, ret_js->valueint);
            }
        }
    }

Error:
    if (root != NULL)
        cJSON_Delete(root);

    return;
}
#endif


int main(int argc, char const *argv[])
{
    // _test_json();
    // leshi_sure_band();
    // leshi_datapoint_parse();
    // leshi_ctrl_switch_dev();
    // leshi_delete_one_dev();
    // leshi_addto_group();
    // leshi_group_ctrl_devs();
    // leshi_delfrom_group();
    // leshi_heart_beat();

    return 0;
}
