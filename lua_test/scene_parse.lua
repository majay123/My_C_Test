--[[

************Copyright 2025 MCD************

version      : 
Company      : HOPE
Author       : MCD
Date         : 2024-01-31 14:39:31
LastEditors  : MCD
LastEditTime : 2025-04-22 16:46:41
FilePath     : /My_C_Test/lua_test/scene_parse.lua
Description  : 

******************************************
--]]
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
Date         : 2024-01-31 14:39:31
LastEditors  : MCD
LastEditTime : 2024-01-31 14:39:32
FilePath     : /My_C_Test/lua_test/scene_parse.lua
Description  :



******************************************
--]]
package.path = '/mnt/xfont/lua/?.lua'
local json = require("cjson.safe")
-- require("utils")

local json_buf = '[{"objectName":"我回家了","type":"http","objectId":"1565870559501021184","platformId":"750837261197414400"},{"objectName":"灯光全开","type":"http","objectId":"1565870245104381952","platformId":"750837261197414400"},{"objectName":"我出门了","type":"http","objectId":"1565870618254811136","platformId":"750837261197414400"},{"objectName":"灯光全关","type":"http","objectId":"1565870316713734144","platformId":"750837261197414400"},{"objectName":"暂停","type":"http","objectId":"1578215742074843136","platformId":"750837261197414400"},{"objectName":"开灯","type":"http","objectId":"1576759193976479744","platformId":"750837261197414400"},{"objectName":"播放","type":"http","objectId":"1577761978687729664","platformId":"750837261197414400"},{"objectName":"回家了","type":"http","objectId":"1578502325776732160","platformId":"750837261197414400"}]'

local json_buf1 = '["002qU5aY3Qu24y","001zMQr71F1Qo8","004Z8Ihr0JIu5s","001xd0HI0X9GNq","001Bbywq2gicae","0039MnYb0qxYhV","000oW8J53xPhZA","00128N3r2SYKMF","0042QMDR1VzSsx","003cI52o4daJJL","003KtYhg4frNXC","0017K7gL4WYnw2","00176bPZ2wu39R","004Yi5BD3ksoAN","0022b7OX2STU86","001L1lqm4UAdyo","001K0AjL2huSxx","0009BCJK1nRaad","003xv4w313tZHV","002PmJmr3qy031","004emQMs09Z1lz","002MXZNu1GToOk","002usg9o4GTAKf","002OKIox28ad9a","004BhQke4adHcf","003ZrdgZ0UnjDl","00265Jxe3JzXOJ","001qvvgF38HVc4","000sxzol11raSd","0027oMO61wWi55"]'


local scene_path = '/mnt/xfont/scenes_list.json'

local function hope_smarthome_find_scene_name(scene_name)
    print("读取场景配置")
    -- local s = utils.readfile(scene_path)

    if not json_buf or #json_buf <= 0 then 
        print("未发现匹配场景")
        return nil 
    end

    print("解析&读取场景")
    local tab = json.decode(json_buf)

    for i, v in pairs(tab) do
       print(v["objectName"])
       if string.match(v["objectName"], scene_name) then
           print(v["objectName"])
           return nil
       else 
           print("no match")
       end
    end
end

-- local scene_name = "回家了"
-- hope_smarthome_find_scene_name(scene_name)
-- if string.match(scene_name, "回家了") then
--     print("回家")
-- else    
--     print("no match")
-- end

local tab = json.decode(json_buf1)
local result = table.concat(tab, ",")
-- for i, v in pairs(tab) do
--     print(v)
-- end
print(result)

