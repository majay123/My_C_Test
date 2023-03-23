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
 * @Date         : 2023-03-22 16:20:39
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-23 13:54:29
 * @FilePath     : /My_C_Test/Hope_Work/leshi/ls_bt_mesh_cmd_send.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "ls_bt_mesh_cmd_send.h"
#include "common.h"
#include "debug.h"
#include "ls_bt_cmd_dispatch.h"
#include "ls_bt_mesh.h"
#include "ls_bt_mesh_dev.h"
#include "queue.h"

#if 0
static int _ls_send_cmd_get_device_info(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_allow_sub_device_join(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_close_sub_device_join(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_sub_device_add(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_sub_device_delete(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_heartbeat_check(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_sub_devices_status_query(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_sub_device_status_report(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_group_add_sub_devices(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_group_delete_sub_devices(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_get_system_date(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_get_local_date(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_batch_add_sub_devices(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_return_to_add_device_results(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_control_group_devices(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_get_wifi_status(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_restore_factory_settings(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_mcu_startup_upgrade(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);
static int _ls_send_cmd_mcu_upgrade_package_transfer(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len);

const ls_send_cmd_list_t leshi_send_cmd_list[] =
{
    {LESHI_CMD_DEV_INFO                     , (send_cmd_callback)_ls_send_cmd_get_device_info},
    {LESHI_CMD_ALLOW_SUB_DEVICE_JOIN        , (send_cmd_callback)_ls_send_cmd_allow_sub_device_join},
    {LESHI_CMD_CLOSE_SUB_DEVICE_JOIN        , (send_cmd_callback)_ls_send_cmd_close_sub_device_join},
    {LESHI_CMD_SUB_DEVICE_ADD               , (send_cmd_callback)_ls_send_cmd_sub_device_add},
    {LESHI_CMD_SUB_DEVICE_DELETE            , (send_cmd_callback)_ls_send_cmd_sub_device_delete},
    {LESHI_CMD_HEARTBEAT_CHECK              , (send_cmd_callback)_ls_send_cmd_heartbeat_check},
    {LESHI_CMD_SUB_DEVICES_STATUS_QUERY     , (send_cmd_callback)_ls_send_cmd_sub_devices_status_query},
    {LESHI_CMD_SUB_DEVICES_STATUS_REPORT    , (send_cmd_callback)_ls_send_cmd_sub_device_status_report},
    {LESHI_CMD_GROUP_ADD_SUB_DEVICES        , (send_cmd_callback)_ls_send_cmd_group_add_sub_devices},
    {LESHI_CMD_GROUP_DELETE_SUB_DEVICES     , (send_cmd_callback)_ls_send_cmd_group_delete_sub_devices},
    {LESHI_CMD_GET_SYSTEM_DATE              , (send_cmd_callback)_ls_send_cmd_get_system_date},
    {LESHI_CMD_GET_LOCAL_DATE               , (send_cmd_callback)_ls_send_cmd_get_local_date},
    {LESHI_CMD_BATCH_ADD_SUB_DEVICES        , (send_cmd_callback)_ls_send_cmd_batch_add_sub_devices},
    {LESHI_CMD_RETURN_TO_ADD_DEVICE_RESULTS , (send_cmd_callback)_ls_send_cmd_return_to_add_device_results},
    {LESHI_CMD_CONTROL_GROUP_DEVICES        , (send_cmd_callback)_ls_send_cmd_control_group_devices},
    {LESHI_CMD_GET_WIFI_STATUS              , (send_cmd_callback)_ls_send_cmd_get_wifi_status},
    {LESHI_CMD_RESTORE_FACTORY_SETTINGS     , (send_cmd_callback)_ls_send_cmd_restore_factory_settings},
    {LESHI_CMD_MCU_STARTUP_UPGRADE          , (send_cmd_callback)_ls_send_cmd_mcu_startup_upgrade},
    {LESHI_CMD_MCU_UPGRADE_PACKAGE_TRANSFER , (send_cmd_callback)_ls_send_cmd_mcu_upgrade_package_transfer},

    /* 函数指针为NULL表示最后一项*/
    {0, NULL},
};

static int _ls_send_cmd_get_device_info(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    int ret = -1;

    ret = _ls_bt_mesh_cmd_send(llq_handle, LESHI_CMD_DEV_INFO, NULL, 0);

    return ret;
}
static int _ls_send_cmd_allow_sub_device_join(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    int ret = -1;

    ret = _ls_bt_mesh_cmd_send(llq_handle, LESHI_CMD_SUB_DEVICE_DELETE, cdata, cdata_len);

    return ret;
}
static int _ls_send_cmd_close_sub_device_join(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_sub_device_add(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_sub_device_delete(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_heartbeat_check(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_sub_devices_status_query(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_sub_device_status_report(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_group_add_sub_devices(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_group_delete_sub_devices(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_get_system_date(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_get_local_date(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_batch_add_sub_devices(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_return_to_add_device_results(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_control_group_devices(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_get_wifi_status(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_restore_factory_settings(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_mcu_startup_upgrade(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
static int _ls_send_cmd_mcu_upgrade_package_transfer(llq_t *llq_handle, leshi_cmd_e cmd, const uint8_t *cdata, size_t cdata_len)
{
    return 0;
}
#endif

/**
* @details  	将发送的数据送入发送队列
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-23-13:23 V1.0.0				MCD				create
*/
static int _ls_bt_mesh_add_queue(llq_t *llq_handle, uint8_t *data, size_t data_size)
{
    ls_mesh_dev_data_t send_msg;

    if ((data == NULL) || (data_size < LESHI_DATA_HEADER_LEN)) {
        DEBUG_ERROR("data is NULL or data_size is negative!!");
        return -1;
    }

    memset(&send_msg, 0, sizeof(send_msg));

    send_msg.data_len = data_size;
    send_msg.data = calloc(send_msg.data_len, sizeof(uint8_t));
    if (send_msg.data) {
        memcpy(send_msg.data, data, send_msg.data_len);
        if (appContext.verbose)
            DEBUG_INFO("llq dat len = %d", send_msg.data_len);
        if (llq_add(llq_handle, (char *)send_msg.data, send_msg.data_len, 0) < 0) {
            DEBUG_ERROR("llq_handler add error\n");
            free(send_msg.data);
            return -1;
        }
    }
    else {
        DEBUG_ERROR("llq data calloc failed!");
        return -1;
    }

    return 0;
}

/**
* @details  	封装成串口发送的数据
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-23-13:23 V1.0.0				MCD				create
*/
int ls_bt_mesh_cmd_send(llq_t *llq_handle, leshi_cmd_e cmd, uint8_t *cdata, size_t cdata_size)
{
    leshi_mesh_header_t header;
    int i = 0;
    uint32_t sum = 0;
    uint8_t *send_data = NULL;
    uint8_t crc = 0;
    int ret = -1;

    memset(&header, 0, sizeof(header));

    header.head[0] = LESHI_DATA_HEADER1;
    header.head[1] = LESHI_DATA_HEADER2;
    header.version = LESHI_DATA_VERSION;
    header.cmd = cmd;
    header.data_len[0] = HI_UINT16(cdata_size);
    header.data_len[1] = LO_UINT16(cdata_size);

    uint8_t *tmp_data = (uint8_t *)&header;
    for (i = 0; i < sizeof(leshi_mesh_header_t); i++) {
        sum += tmp_data[i];
    }
    if (cdata_size > 0 && (NULL != cdata)) {
        for (i = 0; i < cdata_size; i++) {
            sum += cdata[i];
        }
    }

    crc = sum % 256;

    size_t sdata_size = sizeof(leshi_mesh_header_t) + cdata_size + 1;
    send_data = (uint8_t *)calloc(sdata_size, sizeof(uint8_t));
    REQUIRE(NULL == send_data, Error);
    if (send_data) {
        memcpy(send_data, &header, sizeof(leshi_mesh_header_t));
        if (cdata_size > 0 && (NULL != cdata)) 
            memcpy(send_data + sizeof(leshi_mesh_header_t), cdata, cdata_size);
        send_data[sdata_size] = crc;
        ret = _ls_bt_mesh_add_queue(llq_handle, send_data, sdata_size);
        if (ret < 0) {
            DEBUG_ERROR("add queue failed!");
        }
    }
Error:
    if(send_data)
        free(send_data);

    return ret;
}

#if 0
/**
* @details  	处理发送数据的函数
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-23-13:22 V1.0.0				MCD				create
*/
int ls_bt_mesh_send_cmd_process(llq_t *llq_handle, leshi_cmd_e cmd, uint8_t *cdata, size_t cdata_size)
{
    int i = 0;
    int ret = -1;

    DEBUG_INFO("need process cmd=>[0x%02x]", cmd);

    for (i = 0; i < NELEM(leshi_send_cmd_list); i++) {
        if (leshi_send_cmd_list[i].cmd_id == cmd) {
            DEBUG_INFO("Execution cmd : 0x%02x", cmd);
            if (leshi_send_cmd_list[i].cmd_cbk != NULL) {
                ret = leshi_send_cmd_list[i].cmd_cbk(llq_handle, cmd, cdata, cdata_size);
            }
            break;
        }
    }
}
#endif