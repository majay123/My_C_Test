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
 * @Date         : 2023-03-18 10:02:29
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-20 16:50:37
 * @FilePath     : /My_C_Test/Hope_Work/leshi/ls_bt_cmd_dispatch.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "ls_bt_cmd_dispatch.h"
#include "cJSON.h"
#include "common.h"

static int _ls_back_cmd_get_device_info(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_allow_sub_device_join(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_close_sub_device_join(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_sub_device_add(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_sub_device_delete(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_heartbeat_check(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_sub_devices_status_query(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_sub_device_status_report(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_group_add_sub_devices(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_group_delete_sub_devices(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_get_system_date(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_get_local_date(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_batch_add_sub_devices(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_return_to_add_device_results(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_control_group_devices(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_get_wifi_status(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_restore_factory_settings(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_mcu_startup_upgrade(const uint8_t *cmd_data, uint16_t cdata_len);
static int _ls_back_cmd_mcu_upgrade_package_transfer(const uint8_t *cmd_data, uint16_t cdata_len);

const ls_command_list_t leshi_cmd_list[] =
    {
        {LESHI_CMD_DEV_INFO, (command_callback)_ls_back_cmd_get_device_info},
        {LESHI_CMD_ALLOW_SUB_DEVICE_JOIN, (command_callback)_ls_back_cmd_allow_sub_device_join},
        {LESHI_CMD_CLOSE_SUB_DEVICE_JOIN, (command_callback)_ls_back_cmd_close_sub_device_join},
        {LESHI_CMD_SUB_DEVICE_ADD, (command_callback)_ls_back_cmd_sub_device_add},
        {LESHI_CMD_SUB_DEVICE_DELETE, (command_callback)_ls_back_cmd_sub_device_delete},
        {LESHI_CMD_HEARTBEAT_CHECK, (command_callback)_ls_back_cmd_heartbeat_check},
        {LESHI_CMD_SUB_DEVICES_STATUS_QUERY, (command_callback)_ls_back_cmd_sub_devices_status_query},
        {LESHI_CMD_SUB_DEVICES_STATUS_REPORT, (command_callback)_ls_back_cmd_sub_device_status_report},
        {LESHI_CMD_GROUP_ADD_SUB_DEVICES, (command_callback)_ls_back_cmd_group_add_sub_devices},
        {LESHI_CMD_GROUP_DELETE_SUB_DEVICES, (command_callback)_ls_back_cmd_group_delete_sub_devices},
        {LESHI_CMD_GET_SYSTEM_DATE, (command_callback)_ls_back_cmd_get_system_date},
        {LESHI_CMD_GET_LOCAL_DATE, (command_callback)_ls_back_cmd_get_local_date},
        {LESHI_CMD_BATCH_ADD_SUB_DEVICES, (command_callback)_ls_back_cmd_batch_add_sub_devices},
        {LESHI_CMD_RETURN_TO_ADD_DEVICE_RESULTS, (command_callback)_ls_back_cmd_return_to_add_device_results},
        {LESHI_CMD_CONTROL_GROUP_DEVICES, (command_callback)_ls_back_cmd_control_group_devices},
        {LESHI_CMD_GET_WIFI_STATUS, (command_callback)_ls_back_cmd_get_wifi_status},
        {LESHI_CMD_RESTORE_FACTORY_SETTINGS, (command_callback)_ls_back_cmd_restore_factory_settings},
        {LESHI_CMD_MCU_STARTUP_UPGRADE, (command_callback)_ls_back_cmd_mcu_startup_upgrade},
        {LESHI_CMD_MCU_UPGRADE_PACKAGE_TRANSFER, (command_callback)_ls_back_cmd_mcu_upgrade_package_transfer},

        /* 函数指针为NULL表示最后一项*/
        {0, NULL},
};

/**
* @details  	_ls_back_cmd_get_data_string
* @param[in]	
* @param[out]	返回cmd data len
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-15:39 V1.0.0				MCD				create
*/
static uint16_t _ls_back_cmd_get_data_len(const uint8_t *data)
{
    leshi_mesh_header_t header;
    uint16_t cdata_len = 0;

    memset(&header, 0, sizeof(header));
    memcpy(&header, data, sizeof(header));
    cdata_len = BUILD_UINT16(header.data_len[1], header.data_len[0]);

    return cdata_len;
}

/**
* @details  	_ls_back_cmd_get_cmd
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-20-09:08 V1.0.0				MCD				create
*/
static uint8_t _ls_back_cmd_get_cmd(const uint8_t *data)
{
    leshi_mesh_header_t header;

    memset(&header, 0, sizeof(header));
    memcpy(&header, data, sizeof(header));

    return header.cmd;
}

/**
* @details  	_ls_back_cmd_show_dp
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-20-09:26 V1.0.0				MCD				create
*/
static void _ls_back_cmd_show_dp(_leshi_dp_t *dp)
{
    if (NULL == dp)
        return;

    print_mcd("dpid = %d, type = %d, len = %d ", dp->dpid, dp->type, BUILD_UINT16(dp->len[1], dp->len[0]));
    switch (dp->type) {
        case DP_TYPE_RAW:
            print_mcd("this is type raw!");
            break;
        case DP_TYPE_BOOL:
        case DP_TYPE_ENUM:
            print_mcd("value = %d\n", dp->value[0]);
            break;
        case DP_TYPE_VALUE:
            print_mcd("value %d\n", BUILD_UINT32(dp->value[3], dp->value[2], dp->value[1], dp->value[0]));
            break;
        case DP_TYPE_STRING:
            print_mcd("value %s\n", dp->value);
            break;
        case DP_TYPE_BITMAP:
            print_mcd("this is type bitmap!");
            break;

        default:
            break;
    }
}

static void _ls_datapoint_data_free(leshi_ctrl_data_t *dp_data)
{
    int i = 0;

    REQUIRE(NULL == dp_data, Error);
    for (i = 0; i < dp_data->dp_num; i++) {
        if (dp_data->datapoint[i].value)
            free(dp_data->datapoint[i].value);
    }
    if (dp_data->datapoint)
        free(dp_data->datapoint);
    if (dp_data->sub_id)
        free(dp_data->sub_id);
    if (dp_data)
        free(dp_data);

Error:
    return;
}

/**
* @details  	_ls_datapoint_parse
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-20-10:14 V1.0.0				MCD				create
*/
static leshi_ctrl_data_t *_ls_datapoint_parse(const uint8_t *cmd_data, uint16_t cdata_len)
{
    leshi_ctrl_data_t *ctrl_data = NULL;
    _leshi_dp_t *dp = NULL;
    uint16_t len = 0;
    uint16_t tmp_len = 0;
    uint8_t dp_num = 0;
    uint8_t i = 0;

    ctrl_data = (leshi_ctrl_data_t *)calloc(1, sizeof(leshi_ctrl_data_t));
    REQUIRE(NULL == ctrl_data, Error);
    ctrl_data->id_len = cmd_data[0];
    len++;

    // 实际长度是10，但是需要多申请一位内存
    // 如果不多申请一位，可能会导致溢出
    ctrl_data->sub_id = (uint8_t *)calloc(ctrl_data->id_len + 1, sizeof(uint8_t));
    REQUIRE(NULL == ctrl_data->sub_id, Error);

    memcpy(ctrl_data->sub_id, &cmd_data[1], ctrl_data->id_len);
    len += ctrl_data->id_len;
    print_mcd("sub id = %s, len = %d", ctrl_data->sub_id, len);
    tmp_len = len;

    /* get dp num */
    _leshi_dp_t tdp;
    while (cdata_len > tmp_len) {
        memcpy(&tdp, &cmd_data[tmp_len], LESHI_DATAPOINT_HEADER_LEN);
        tmp_len += LESHI_DATAPOINT_HEADER_LEN;
        tmp_len += BUILD_UINT16(tdp.len[1], tdp.len[0]);
        memset(&tdp, 0, sizeof(_leshi_dp_t));
        dp_num++;
    }
    print_mcd("get dp num = %d\n", dp_num);

    ctrl_data->dp_num = dp_num;
    if (dp_num > 0) {
        ctrl_data->datapoint = (_leshi_dp_t *)calloc(dp_num, sizeof(_leshi_dp_t));
        REQUIRE(NULL == ctrl_data->datapoint, Error);
        i = 0;
        while (cdata_len > len) {
            dp = &ctrl_data->datapoint[i];
            memcpy(dp, &cmd_data[len], 4);
            len += 4;
            uint16_t dp_len = BUILD_UINT16(dp->len[1], dp->len[0]);
            dp->value = calloc(dp_len + 1, sizeof(uint8_t));
            REQUIRE(NULL == dp->value, Error);
            memcpy(dp->value, &cmd_data[len], dp_len);
            len += dp_len;
            i++;
        }
#if 1
        for (i = 0; i < dp_num; i++) {
            _ls_back_cmd_show_dp(&ctrl_data->datapoint[i]);
        }
#endif
    }

    printf("finally len = %d\n", len);

    return ctrl_data;

Error:
    _ls_datapoint_data_free(ctrl_data);

    return NULL;
}

/**
* @details  	查询产品信息,乐式返回(MCU), LESHI_CMD_DEV_INFO 0x01
*               1.product  ID：产品标识  PID，用于记录产品相关信息
*               2.产品信息由  product  ID、MCU  软件版本构成
*               3.MCU 软件版本号格式定义：采用点分十进制形式，”x.x.x”（0<=x<=99），x 为十进制数
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:40 V1.0.0				MCD				create
*/
static int _ls_back_cmd_get_device_info(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = -1;
    cJSON *root = NULL;

    REQUIRE(NULL == cmd_data, Error);

    root = REQ_JSON_PARSE((const char *)cmd_data, root, Error);

    /* Processe cjson information */

    ret = 0;

Error:
    if (root != NULL)
        cJSON_Delete(root);

    return ret;
}

/**
* @details  	允许子设备入网,乐式返回(MCU), LESHI_CMD_ALLOW_SUB_DEVICE_JOIN 0x06
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:39 V1.0.0				MCD				create
*/
static int _ls_back_cmd_allow_sub_device_join(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;

    return ret;
}

/**
* @details  	关闭子设备入网,乐式返回(MCU), LESHI_CMD_CLOSE_SUB_DEVICE_JOIN 0x07
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:39 V1.0.0				MCD				create
*/
static int _ls_back_cmd_close_sub_device_join(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;

    return ret;
}

/**
* @details  	子设备添加,乐式返回(MCU), LESHI_CMD_SUB_DEVICE_ADD 0x08
*               数据中:
*               0x00  接收请求
*               0x01  拒绝请求
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:38 V1.0.0				MCD				create
*/
static int _ls_back_cmd_sub_device_add(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = -1;
    cJSON *root = NULL;

    REQUIRE(NULL == cmd_data, Error);

    root = REQ_JSON_PARSE((const char *)cmd_data, root, Error);

    /* Processe cjson information */

    ret = 0;

Error:
    if (root != NULL)
        cJSON_Delete(root);

    return ret;
}

/**
* @details  	子设备删除,乐式返回(MCU), LESHI_CMD_SUB_DEVICE_DELETE 0x09
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:38 V1.0.0				MCD				create
*/
static int _ls_back_cmd_sub_device_delete(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;

    return ret;
}

/**
* @details  	心跳检测,乐式返回(MCU), LESHI_CMD_HEARTBEAT_CHECK 0x0A
*               {“sub_id”:“xxxx”, “hb_time”:xx}
*               sub_id：子设备  ID
*               心跳间隔 hb_time（单位： 秒）:如：60表示60秒，0不
*               支持心跳，永久在线。若心跳间隔小于30s当30s处理（除 0以外）
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:38 V1.0.0				MCD				create
*/
static int _ls_back_cmd_heartbeat_check(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = -1;
    cJSON *root = NULL;

    REQUIRE(NULL == cmd_data, Error);

    root = REQ_JSON_PARSE((const char *)cmd_data, root, Error);

    /* Processe cjson information */

    ret = 0;

Error:
    if (root != NULL)
        cJSON_Delete(root);

    return ret;
}

/**
* @details  	状态查询,乐式返回(MCU), LESHI_CMD_SUB_DEVICES_STATUS_QUERY 0x0B
*               用于同步设子设备状态。
*               id_len(1byte) + sub_id(nbyte)+ datapoint
*               id_len:子设备id的长度
*               sub_id:子设备id当sub_id为”0000 ”时，代表是上报网关本身的 dp（主设备）。
*               datapoint:状态命令数据点集合
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:36 V1.0.0				MCD				create
*/
static int _ls_back_cmd_sub_devices_status_query(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;
    leshi_ctrl_data_t *dp_data = NULL;

    dp_data = _ls_datapoint_parse(cmd_data, cdata_len);
    REQUIRE(dp_data == NULL, Error);

    /* Processe datapoint information */

    _ls_datapoint_data_free(dp_data);
Error:
    return ret;
}

/**
* @details  	状态上报,乐式返回(MCU), LESHI_CMD_SUB_DEVICES_STATUS_REPORT 0x0D
*               id_len(1byte) + sub_id(nbyte)+ datapoint
*               id_len:子设备id的长度
*               sub_id:子设备id当sub_id为”0000 ”时，代表是上报网关本身的 dp（主设备）。
*               datapoint:状态命令数据点集合
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:36 V1.0.0				MCD				create
*/
static int _ls_back_cmd_sub_device_status_report(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;
    leshi_ctrl_data_t *dp_data = NULL;

    dp_data = _ls_datapoint_parse(cmd_data, cdata_len);
    REQUIRE(dp_data == NULL, Error);

    /* Processing datapoint information */

    _ls_datapoint_data_free(dp_data);
Error:
    return ret;
}

static void _ls_back_cmd_group_add_sub_devices_status_string(char *group, char *cid, int rest)
{
    switch (rest) {
        case LS_GROUP_ADD_SUB_DEVICE_SUCCESS:
            print_mcd("group [%s], add sub device [%s] success!!", group, cid);
            break;
        case LS_GROUP_ADD_SUB_DEVICE_OUT_UPPER_LIMIT:
            print_mcd("group [%s], add sub device [%s] exceed the upper limit of the number of groups!!", group, cid);
            break;
        case LS_GROUP_ADD_SUB_DEVICE_TIMEOUT:
            print_mcd("group [%s], add sub device [%s] timeout!!", group, cid);
            break;
        case LS_GROUP_ADD_SUB_DEVICE_SET_OUT_OF_RANGE:
            print_mcd("group [%s], add sub device [%s] setting value out of range!!", group, cid);
            break;
        case LS_GROUP_ADD_SUB_DEVICE_WRITE_FILE_ERR:
            print_mcd("group [%s], add sub device [%s] written file error!!", group, cid);
            break;
        case LS_GROUP_ADD_SUB_DEVICE_OTHER_ERR:
            print_mcd("group [%s], add sub device [%s] other errors!!", group, cid);
            break;

        default:
            break;
    }
}

/**
* @details  	群组子设备加入,乐式返回(MCU), LESHI_CMD_GROUP_ADD_SUB_DEVICES 0x0E
*               {“gid”:”12”,“cids”:[”12”,”34”,”56”], “rets”:[0,0,0]}
*               gid: 表示群组 id
*               cids:要加入群组的子设备集合
*               rest:返回结果结合:
*                   0:添加成功
*                   1:超过群组数上限 
*                   2:子设备超时
*                   3:设置值超出范围 
*                   4:写文件错误
*                   5:其他错误
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:35 V1.0.0				MCD				create
*/
static int _ls_back_cmd_group_add_sub_devices(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = -1;
    cJSON *root = NULL;
    int i = 0;

    REQUIRE(NULL == cmd_data, Error);

    root = REQ_JSON_PARSE((const char *)cmd_data, root, Error);

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
                _ls_back_cmd_group_add_sub_devices_status_string(gid->valuestring, cid_js->valuestring, ret_js->valueint);
            }
        }
    }

    ret = 0;

Error:
    if (root != NULL)
        cJSON_Delete(root);

    return ret;
}

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

/**
* @details  	群组子设备删除,乐式返回(MCU), LESHI_CMD_GROUP_DELETE_SUB_DEVICES 0x0F
*               {“gid”:”12”,“cids”:[”12”,”34”,”56”], “rets”:[0,0,0]}
*               gid: 表示群组 id
*               cids:要加入群组的子设备集合
*               rest:返回结果结合:
*                   0:删除成功
*                   1:超过群组数上限 
*                   2:子设备超时
*                   3:设置值超出范围 
*                   4:写文件错误
*                   5:其他错误
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:34 V1.0.0				MCD				create
*/
static int _ls_back_cmd_group_delete_sub_devices(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = -1;
    cJSON *root = NULL;
    int i = 0;

    REQUIRE(NULL == cmd_data, Error);

    root = REQ_JSON_PARSE((const char *)cmd_data, root, Error);

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

    ret = 0;

Error:
    if (root != NULL)
        cJSON_Delete(root);

    return ret;
}

/**
* @details  	获取系统时间(格林时间),乐式返回(MCU), LESHI_CMD_GET_SYSTEM_DATE 0x10
*               [暂时不处理这个指令]
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:34 V1.0.0				MCD				create
*/
static int _ls_back_cmd_get_system_date(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;

    return ret;
}

/**
* @details  	获取本地时间,乐式返回(MCU), LESHI_CMD_GET_LOCAL_DATE 0x11
*               [暂时不处理这个指令]
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:33 V1.0.0				MCD				create
*/
static int _ls_back_cmd_get_local_date(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;

    return ret;
}

/**
* @details  	批量添加设备,乐式返回(MCU), LESHI_CMD_BATCH_ADD_SUB_DEVICES 0x12
*               {“pid”:” xxxx”,“cids”:[”12”,”34”,”56”], “ver”:”x.x.x”}
*               产品标识pid:产品ID(ProductID),涂鸦云创建产品时生产
*               cids:待添加子设备ID集合
*               版本号  ver:如:1.0.0
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:33 V1.0.0				MCD				create
*/
static int _ls_back_cmd_batch_add_sub_devices(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = -1;
    cJSON *root = NULL;
    int i = 0;

    REQUIRE(NULL == cmd_data, Error);

    root = REQ_JSON_PARSE((const char *)cmd_data, root, Error);

    /* Processe cjson information */
    cJSON *pid = REQ_JSON_OBJ(root, pid, Error);
    cJSON *cids = REQ_JSON_OBJ(root, cids, Error);
    // cJSON *ver = REQ_JSON_OBJ(root, ver, Error);

    if (pid->valuestring) {
        int csize = cJSON_GetArraySize(cids);
        if (csize > 0) {
            for (i = 0; i < csize; i++) {
                cJSON *cid_js = cJSON_GetArrayItem(cids, i);
                print_mcd("cid = %s", cid_js->valuestring);
            }
        }
    }

    ret = 0;

Error:
    if (root != NULL)
        cJSON_Delete(root);

    return ret;
}

/**
* @details  	返回添加设备结果,乐式返回(MCU), LESHI_CMD_RETURN_TO_ADD_DEVICE_RESULTS 0x13
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:32 V1.0.0				MCD				create
*/
static int _ls_back_cmd_return_to_add_device_results(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;

    return ret;
}

/**
* @details  	控制群组指令下发,乐式返回(MCU), LESHI_CMD_CONTROL_GROUP_DEVICES 0x14
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:23 V1.0.0				MCD				create
*/
static int _ls_back_cmd_control_group_devices(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;

    return ret;
}

/**
* @details  	乐式模块获取WIFI状态, LESHI_CMD_GET_WIFI_STATUS 0x16
*               [暂时不处理这个指令]
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:24 V1.0.0				MCD				create
*/
static int _ls_back_cmd_get_wifi_status(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;

    return ret;
}

/**
* @details  	恢复出厂设置(可选), LESHI_CMD_RESTORE_FACTORY_SETTINGS 0x17
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:23 V1.0.0				MCD				create
*/
static int _ls_back_cmd_restore_factory_settings(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;

    return ret;
}

/**
* @details  	MCU(乐式模块)启动升级, LESHI_CMD_MCU_STARTUP_UPGRADE 0x1D
*               [具体看乐式协议文档3.25.1]升级包分包传输大小：
*                   0x00：默认  256byte（兼容旧固件）
*                   0x01：512byte 
*                   0x02：1024byte 
*                   0x03：128byte
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:22 V1.0.0				MCD				create
*/
static int _ls_back_cmd_mcu_startup_upgrade(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;
    uint16_t tr_size = 0;

    if (cdata_len == LESHI_UPGRADE_PACKET_TRANSFER_DATA_LEN) {
        switch (cmd_data[0]) {
            case LS_PACKAGE_SIZE_256_BYTE:
                tr_size = LESHI_UPGRADE_PACKET_TRANSFER_256BY;
                break;
            case LS_PACKAGE_SIZE_512_BYTE:
                tr_size = LESHI_UPGRADE_PACKET_TRANSFER_256BY;
                break;
            case LS_PACKAGE_SIZE_1024_BYTE:
                tr_size = LESHI_UPGRADE_PACKET_TRANSFER_1024BY;
                break;
            case LS_PACKAGE_SIZE_128_BYTE:
                tr_size = LESHI_UPGRADE_PACKET_TRANSFER_128BY;
                break;
            default:
                tr_size = 0;
                ret = -1;
                break;
        }
    }

    return ret;
}

/**
* @details  	MCU(乐式模块) 升级包传输, LESHI_CMD_MCU_UPGRADE_PACKAGE_TRANSFER 0x1E
*               [具体看乐式协议文档3.25.2]
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:21 V1.0.0				MCD				create
*/
static int _ls_back_cmd_mcu_upgrade_package_transfer(const uint8_t *cmd_data, uint16_t cdata_len)
{
    int ret = 0;

    return ret;
}

int ls_bt_back_cmd_dispatch(const uint8_t *data)
{
    int ret = -1;
    uint16_t cdata_len = 0;
    uint8_t *cmd_data = NULL;
    int i = 0;
    uint8_t cmd = 0;

    REQUIRE(NULL == data, Error);

    cdata_len = _ls_back_cmd_get_data_len(data);
    cmd = _ls_back_cmd_get_cmd(data);

    if (cdata_len > 0 && cdata_len <= LESHI_MAX_DATA_LEN) {
        for (i = 0; i < NELEM(leshi_cmd_list); i++) {
            if (leshi_cmd_list[i].cmd_id == cmd) {
                cmd_data = calloc(cdata_len + 1, sizeof(uint8_t));
                REQUIRE(NULL == cmd_data, Error);
                print_mcd("Execution cmd : %s", cmd_data);
                memcpy(cmd_data, &data[LESHI_DATA_HEADER_LEN], cdata_len);
                if (leshi_cmd_list[i].cmd_cbk != NULL) {
                    ret = leshi_cmd_list[i].cmd_cbk(cmd_data, cdata_len);
                }
            }
        }
    }
    else if (cdata_len == 0) {
        for (i = 0; i < NELEM(leshi_cmd_list); i++) {
            if (leshi_cmd_list[i].cmd_id == cmd) {
                print_mcd("Execution cmd : %s", cmd_data);
                if (leshi_cmd_list[i].cmd_cbk != NULL) {
                    ret = leshi_cmd_list[i].cmd_cbk(cmd_data, cdata_len);
                }
            }
        }
    }
    else {
        print_mcd("do nothing cmd=>[0x%02x], just back!!!", cmd);
    }

Error:
    if (cmd_data != NULL)
        free(cmd_data);

    return ret;
}
