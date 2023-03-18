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
 * @LastEditTime : 2023-03-18 15:14:24
 * @FilePath     : /My_C_Test/Hope_Work/leshi/ls_bt_cmd_dispatch.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "ls_bt_cmd_dispatch.h"
#include "cJSON.h"
#include "common.h"

static int _ls_back_cmd_get_device_info(const uint8_t *data);
static int _ls_back_cmd_allow_sub_device_join(const uint8_t *data);
static int _ls_back_cmd_close_sub_device_join(const uint8_t *data);
static int _ls_back_cmd_sub_device_add(const uint8_t *data);
static int _ls_back_cmd_sub_device_delete(const uint8_t *data);
static int _ls_back_cmd_heartbeat_check(const uint8_t *data);
static int _ls_back_cmd_sub_devices_status_query(const uint8_t *data);
static int _ls_back_cmd_sub_device_status_report(const uint8_t *data);
static int _ls_back_cmd_group_add_sub_devices(const uint8_t *data);
static int _ls_back_cmd_group_delete_sub_devices(const uint8_t *data);
static int _ls_back_cmd_get_system_date(const uint8_t *data);
static int _ls_back_cmd_get_local_date(const uint8_t *data);
static int _ls_back_cmd_batch_add_sub_devices(const uint8_t *data);
static int _ls_back_cmd_return_to_add_device_results(const uint8_t *data);
static int _ls_back_cmd_control_group_devices(const uint8_t *data);
static int _ls_back_cmd_get_wifi_status(const uint8_t *data);
static int _ls_back_cmd_restore_factory_settings(const uint8_t *data);
static int _ls_back_cmd_mcu_startup_upgrade(const uint8_t *data);
static int _ls_back_cmd_mcu_upgrade_package_transfer(const uint8_t *data);

const ls_command_list_t leshi_cmd_list[] =
{
    {LESHI_CMD_DEV_INFO                     , (command_callback)_ls_back_cmd_get_device_info},
    {LESHI_CMD_ALLOW_SUB_DEVICE_JOIN        , (command_callback)_ls_back_cmd_allow_sub_device_join},
    {LESHI_CMD_CLOSE_SUB_DEVICE_JOIN        , (command_callback)_ls_back_cmd_close_sub_device_join},
    {LESHI_CMD_SUB_DEVICE_ADD               , (command_callback)_ls_back_cmd_sub_device_add},
    {LESHI_CMD_SUB_DEVICE_DELETE            , (command_callback)_ls_back_cmd_sub_device_add},
    {LESHI_CMD_HEARTBEAT_CHECK              , (command_callback)_ls_back_cmd_heartbeat_check},
    {LESHI_CMD_SUB_DEVICES_STATUS_QUERY     , (command_callback)_ls_back_cmd_sub_devices_status_query},
    {LESHI_CMD_SUB_DEVICES_STATUS_REPORT    , (command_callback)_ls_back_cmd_sub_device_status_report},
    {LESHI_CMD_GROUP_ADD_SUB_DEVICES        , (command_callback)_ls_back_cmd_group_add_sub_devices},
    {LESHI_CMD_GROUP_DELETE_SUB_DEVICES     , (command_callback)_ls_back_cmd_group_add_sub_devices},
    {LESHI_CMD_GET_SYSTEM_DATE              , (command_callback)_ls_back_cmd_get_system_date},
    {LESHI_CMD_GET_LOCAL_DATE               , (command_callback)_ls_back_cmd_get_local_date},
    {LESHI_CMD_BATCH_ADD_SUB_DEVICES        , (command_callback)_ls_back_cmd_batch_add_sub_devices},
    {LESHI_CMD_RETURN_TO_ADD_DEVICE_RESULTS , (command_callback)_ls_back_cmd_return_to_add_device_results},
    {LESHI_CMD_CONTROL_GROUP_DEVICES        , (command_callback)_ls_back_cmd_control_group_devices},
    {LESHI_CMD_GET_WIFI_STATUS              , (command_callback)_ls_back_cmd_get_wifi_status},
    {LESHI_CMD_RESTORE_FACTORY_SETTINGS     , (command_callback)_ls_back_cmd_restore_factory_settings},
    {LESHI_CMD_MCU_STARTUP_UPGRADE          , (command_callback)_ls_back_cmd_mcu_startup_upgrade},
    {LESHI_CMD_MCU_UPGRADE_PACKAGE_TRANSFER , (command_callback)_ls_back_cmd_mcu_upgrade_package_transfer},

    /* 函数指针为NULL表示最后一项*/
    {0, NULL},
};

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
static int _ls_back_cmd_get_device_info(const uint8_t *data)
{
    int ret = -1;
    leshi_mesh_header_t header;
    uint16_t cdata_len = 0;
    uint8_t *cmd_data = NULL;
    cJSON *root = NULL;

    memset(&header, 0, sizeof(header));
    memcpy(&header, data, sizeof(header));
    cdata_len = BUILD_UINT16(header.data_len[1], header.data_len[0]);

    if(cdata_len > 0 && cdata_len <= LESHI_MAX_DATA_LEN) {
        cmd_data = calloc(cdata_len + 1, sizeof(uint8_t));
        REQUIRE(cmd_data == NULL, Error);
        print_mcd("get cmd data: %s", cmd_data);
        memcpy(cmd_data, &data[LESHI_DATA_HEADER_LEN], cdata_len);
        root = REQ_JSON_PARSE(cmd_data, root, Error);

        ret = 0;
    }

Error:   
    if(root != NULL) 
        cJSON_Delete(root);
    if(cmd_data != NULL)
        free(cmd_data);
    
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
static int _ls_back_cmd_allow_sub_device_join(const uint8_t *data)
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
static int _ls_back_cmd_close_sub_device_join(const uint8_t *data)
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
static int _ls_back_cmd_sub_device_add(const uint8_t *data)
{
    int ret = 0;

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
static int _ls_back_cmd_sub_device_delete(const uint8_t *data)
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
static int _ls_back_cmd_heartbeat_check(const uint8_t *data)
{
    int ret = 0;

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
static int _ls_back_cmd_sub_devices_status_query(const uint8_t *data)
{
    int ret = 0;

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
static int _ls_back_cmd_sub_device_status_report(const uint8_t *data)
{
    int ret = 0;

    return ret;
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
static int _ls_back_cmd_group_add_sub_devices(const uint8_t *data)
{
    int ret = 0;

    return ret;
}

/**
* @details  	群组子设备删除,乐式返回(MCU), LESHI_CMD_GROUP_DELETE_SUB_DEVICES 0x0F
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
* 2023-03-18-11:34 V1.0.0				MCD				create
*/
static int _ls_back_cmd_group_delete_sub_devices(const uint8_t *data)
{
    int ret = 0;

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
static int _ls_back_cmd_get_system_date(const uint8_t *data)
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
static int _ls_back_cmd_get_local_date(const uint8_t *data)
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
static int _ls_back_cmd_batch_add_sub_devices(const uint8_t *data)
{
    int ret = 0;

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
static int _ls_back_cmd_return_to_add_device_results(const uint8_t *data)
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
static int _ls_back_cmd_control_group_devices(const uint8_t *data)
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
static int _ls_back_cmd_get_wifi_status(const uint8_t *data)
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
static int _ls_back_cmd_restore_factory_settings(const uint8_t *data)
{
    int ret = 0;

    return ret;
}

/**
* @details  	MCU(乐式模块)启动升级, LESHI_CMD_MCU_STARTUP_UPGRADE 0x1D
*               [具体看乐式协议文档3.25.1]
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-18-11:22 V1.0.0				MCD				create
*/
static int _ls_back_cmd_mcu_startup_upgrade(const uint8_t *data)
{
    int ret = 0;

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
static int _ls_back_cmd_mcu_upgrade_package_transfer(const uint8_t *data)
{
    int ret = 0;

    return ret;
}

