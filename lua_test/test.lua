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



************Copyright 2024 MCD************

version      : 
Company      : HOPE
Author       : MCD
Date         : 2024-12-19 11:45:30
LastEditors  : MCD
LastEditTime : 2024-12-19 12:47:33
FilePath     : /My_C_Test/lua_test/test.lua
Description  : 

******************************************
--]]

local function extract_key_and_number(str)
    -- 匹配标识（songlist/toplist/singer）和数字
    local key, number = string.match(str, "(%a+)%-(%w+)")
    return key, number
end

local k, num = extract_key_and_number("singer-23123")
print(k)
print(num, type(num))

local ids = { "qq003QspBx2GGIRv", "003QspBx2GGIRv", "qq001zMQr71F1Qo8" }

for i, id in ipairs(ids) do
    ids[i] = string.gsub(id, "^qq", "")
end

for _, id in ipairs(ids) do
    print(id)
end

local dir = "/mnt/xfont/current_mlist/"
local cmd_del = 'rm -rf '..dir..'*'.. ' && sync'
print(cmd_del)
-- os.execute(cmd_del)

