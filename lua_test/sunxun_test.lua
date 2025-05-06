--[[

　　┏┓　　　┏┓+ +
　┏┛┻━━━┛┻┓ + +
　┃　　　　　　　┃ 　
　┃　　　━　　　┃ ++ + + +
 ████━████ ┃+
　┃　　　　　　　┃ +
　┃　　　┻　　　┃
　┃　　　　　　　┃ + +
　┗━┓　　　┏━┛
　　　┃　　　┃　　　　　　　　　　　
　　　┃　　　┃ + + + +
　　　┃　　　┃
　　　┃　　　┃ +  神兽保佑
　　　┃　　　┃    代码无bug　　
　　　┃　　　┃　　+　　　　　　　　　
　　　┃　 　　┗━━━┓ + +
　　　┃ 　　　　　　　┣┓
　　　┃ 　　　　　　　┏┛
　　　┗┓┓┏━┳┓┏┛ + + + +
　　　　┃┫┫　┃┫┫
　　　　┗┻┛　┗┻┛+ + + +



************Copyright 2025 MCD************

version      : 
Company      : HOPE
Author       : MCD
Date         : 2025-04-01 13:05:54
LastEditors  : MCD
LastEditTime : 2025-04-01 13:05:54
FilePath     : /My_C_Test/lua_test/sunxun_test.lua
Description  : 

******************************************
--]]
local json = require("cjson.safe")

local function qqmusic_data_ascending_order(params)
    -- 获取所有键
    local keys = {}
    for k in pairs(params) do
        table.insert(keys, k)
    end

    -- 对键进行升序排序
    table.sort(keys)

    -- 构造排序后的参数字符串
    local sorted_params = {}
    for _, k in ipairs(keys) do
        table.insert(sorted_params, k .. "=" .. params[k])
    end

    -- 将表转换为以 & 分隔的字符串
    local result = table.concat(sorted_params, "&")

    return result
end

local params = {}
local params1 = {}
local params2 = {}

-- CUSTOM_GET_SONG_INFO_BATCH
-- params.app_id='%s'
-- params.client_ip='%s'
-- params.device_id='%s'
-- params.login_type=5
-- params.opi_cmd='%s'
-- params.qqmusic_access_token='%s'
-- params.qqmusic_open_appid='%s'
-- params.qqmusic_open_id='%s'
-- params.sign_version='v2'
-- params.song_mid='%s'
-- params.timestamp='%s'
-- params.device_login_type=4
-- params.user_login_type=6
-- params.opi_device_key='%s'
-- params.opi_device_id='%s'

-- CUSTOM_SEARCH
-- params.app_id='%s'
-- params.client_ip='%s'
-- params.device_id='%s'
-- params.login_type=5
-- params.num=30
-- params.opi_cmd='%s'
-- params.qqmusic_access_token='%s'
-- params.qqmusic_open_appid='%s'
-- params.qqmusic_open_id='%s'
-- params.sign_version='v2'
-- params.timestamp='%ld'
-- params.w='%s'
-- params.device_login_type=4
-- params.user_login_type=6
-- params.opi_device_key='%s'
-- params.opi_device_id='%s'

-- -- CUSTOM_RC_SONGS
-- params1.app_id='%s'
-- params1.client_ip='%s'
-- params1.device_id='%s'
-- params1.login_type=5
-- params1.opi_cmd='%s'
-- params1.qqmusic_access_token='%s'
-- params1.qqmusic_open_appid='%s'
-- params1.qqmusic_open_id='%s'
-- params1.sign_version='v2'
-- params1.timestamp='%ld'
-- params1.device_login_type=4
-- params1.user_login_type=6
-- params1.opi_device_key='%s'
-- params1.opi_device_id='%s'


-- -- SKILL_NO_NLP
-- params2.app_id='%s'
-- params2.app_key='%s'
-- params2.client_ip='%s'
-- params2.cmd_params='%s'
-- params2.device_id='%s'
-- params2.login_type=5
-- params2.opi_cmd='%s'
-- params2.opi_protocol_version=1
-- params2.qqmusic_access_token='%s'
-- params2.qqmusic_open_appid='%s'
-- params2.qqmusic_open_id='%s'
-- params2.sign_version='v2'
-- params2.timestamp='%ld'
-- params2.device_login_type=4
-- params2.user_login_type=6
-- params2.opi_device_key='%s'
-- params2.opi_device_id='%s'

-- print(qqmusic_data_ascending_order(params))
-- print(qqmusic_data_ascending_order(params1))
-- print(qqmusic_data_ascending_order(params2))

function qqmusic_common_parameters(params, opi_cmd, app_id)
    params.opi_cmd = opi_cmd
    params.app_id = app_id
    params.qqmusic_openid_appId = app_id
    params.device_id = '123456789'
    params.client_ip = 'client_ip'
    params.timestamp = os.time() - 60 * 60 * 8
    params.qqmusic_open_id = '3093jdwfw2'
    params.qqmusic_access_token = 'ewrtfwetgwert'
    params.device_login_type = 4
    params.login_type = 5
    params.user_login_type = 6
end

qqmusic_common_parameters(params, 'cmdagsdag', 'idafgdsagds')


print(json.encode(params))
