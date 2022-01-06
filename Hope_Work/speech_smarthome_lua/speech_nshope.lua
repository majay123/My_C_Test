#!/usr/bin/lua
-- package.cpath = '/usr/local/lib/lua/5.4/cjson.so'

local cjson = require("cjson.safe")


-- speech smarthome api to hope attribute
local sp_to_hope_attr = {
    ['DUI.SmartHome.Device.TurnOn'] = {
        name = 'STATE',
        act = {
            ['DUI.SmartHome.Device.TurnOn'] = 'ON',
        },
    },
    ['DUI.SmartHome.Device.TurnOff'] = {
        name = 'STATE',
        act = {
            ['DUI.SmartHome.Device.TurnOff'] = 'OFF',
        },
    },
    ['DUI.SmartHome.Device.Stop'] = {
        name = 'STATE',
        act = {
            ['DUI.SmartHome.Device.Stop'] = 'STOP'
        },
    },
    ['DUI.SmartHome.SetTemperature'] = {
        sname = 'temperature',
        name = 'TEMPERATURE',
    },
    ['DUI.SmartHome.OpenMode'] = {
        sname = 'mode',
        name = 'MODE',
        act = {
            ['制冷模式'] = 'COLD',
            ['制热模式'] = 'HEAT',
            ['送风模式'] = 'WIND',
            -- ['干燥模式'] = 'DRY',
            ['除湿模式'] = 'DRY',
        },
    },
    ['DUI.SmartHome.SetBrightness'] = {
        sname = 'brightness',
        name = 'LIGHT',
    },
    ['DUI.SmartHome.SetRange'] = {
        sname = 'range',
        name = 'POSITION',
    },
    ['DUI.SmartHome.SetWindSpeed'] = {
        sname = 'windSpeed',
        name = 'WIND',
        act = {
            ['high'] = 'HIGH',
            ['mid'] = 'MEDIUM',
            ['low'] = 'LOW',
            ['auto'] = 'AUTO',
            ['+'] = 'HIGH',
            ['max'] = 'HIGH',
            ['-'] = 'LOW',
            ['min'] = 'LOW',
        };
    },
    ['DUI.SmartHome.SetColor'] = {
        sname = 'color',
        name = 'HUE',
    }
}

local hope_default_act = 'TO'
local hope_act = {
    ['+'] = 'PLUS',
    ['-'] = 'MINUS',
}

local hope_tts_act = {
    TO = '设置为',
    MINUS = '降低',
    PLUS = '提高'
}

local hope_value_to = {
    ['max'] = 'MAX',
    ['min'] = 'MIN',
}

local function chk_num_persent1(str)
    if not str then return str, 0 end
    if tonumber(str) then 
        return tonumber(str), 1
    end
    if str:find('%d+%s*%%') then 
        return tonumber(str:sub(str:find('%d+'))), 2
    elseif str:find('%d+/%d+') then
        local num1 = tonumber(str:sub(str:find('%d+')))
        local str1 = str:sub(str:find('/%d+'));
        local num2 = tonumber(str1:sub(str1:find('%d+')))
        local num3 = math.modf((100 * num1) / num2)
        print(num1, num2, num3)
        return tonumber(num3), 1
    else
        return str, 0
    end
end

-- print("hello world")
-- local sp_param = '{"input":"窗帘开到百分之二十","command":{"param":{"deviceType":"窗帘","range":"20%"},"api":"DUI.SmartHome.SetRange"}}'
-- local sp_param = '{"input":"空调制冷","command":{"param" : {"deviceType" : "空调" ,"mode" : "制冷模式"},"api" : "DUI.SmartHome.OpenMode"}}'
-- local sp_param = '{"input":"空调太冷了","command":{"api":"DUI.SmartHome.SetTemperature","param":{"deviceType":"空调","temperature":"+"}}}'
-- local sp_param = '{"input":"空调25度","command":{"param":{"temperature":"25","deviceType":"空调"},"api":"DUI.SmartHome.SetTemperature"}}'
-- local sp_param = '{"input":"窗帘半开","command":{"param":{"deviceType":"窗帘","range":"50%"},"api":"DUI.SmartHome.SetRange"}}'
-- -- local sp_param = '{"input":"查金壁灯调成红色","command":{"api":"DUI.SmartHome.SetColor","param":{"color":"红","deviceType":"壁灯"}}} '
-- local sp_param = '{"input":"空调风速调大","command":{"api":"DUI.SmartHome.SetWindSpeed","param":{"object":"风速","deviceType":"空调","windSpeed":"+"}}} '
-- local sp_param = '{"input":"查金壁灯调成红色","command":{"api":"DUI.SmartHome.SetColor","param":{"color":"红","deviceType":"壁灯"}}} '
-- local sp_param = '{"input":"窗帘开到四分之一","command":{"param":{"range":"1/4","deviceType":"窗帘"},"api":"DUI.SmartHome.SetRange"}} '

local sp_param = '{"input":"打开空调","command":{"param":{"deviceType":"空调"},"api":"DUI.SmartHome.Device.TurnOn"}}'
-- local sp_param = '{"input":"阅读读灯太暗了","command":{"param":{"deviceType":"阅读灯","brightness":"+"},"api":"DUI.SmartHome.SetBrightness"}}'
local function _get_sp_attribute( param )
    for k, v in pairs(sp_to_hope_attr) do
        -- local p_json = cjson.decode(param)
        local command = param.command
        if command.api == k then
            -- print("setting ", v.name)
            if v.act then
                for _k, _v in pairs(v.act) do
                    if command.api:find(_k) then
                        print(v.name, _v)
                        return v.name, _v, nil
                    else
                        if command.param[v.sname]:find(_k) then
                            print(v.name, _v)
                            return v.name, _v, nil
                        end
                    end
                end
            end
            for _k, _v in pairs(hope_act) do 
                if command.param[v.sname]:find(_k) then
                    local s, e = string.find(param[k], '%d+')
                    local num = (s and string.sub(param[k], s, e)) or nil
                    print(v.name, _v, num)
                    -- local s, e = string.find()
                    return v.name, _v, num
                end
            end
            for _k, _v in pairs(hope_value_to) do
                if command.param[v.sname]:find(_k) then
                    print(v.name, hope_default_act, _v)
                    return v.name, hope_default_act, _v
                end
            end
            local s, e = string.find(tostring(command.param[v.sname]), '%d')
            if s then
                local nu, per = chk_num_persent1(command.param[v.sname])
                -- print(per)
                print(v.name, hope_default_act, tostring(nu))
                return v.name, hope_default_act, tostring(nu)
            else 
                print(v.name, hope_default_act, command.param[v.sname])
                return v.name, hope_default_act, command.param[v.sname]
            end
        end
        -- print(p_json.command.api == )
    end
end


local function speech_sh_to_hope_sh_device(input_tab, ori)
    local ret = { 
        catalog = 'smart',
    }
    print(input_tab.command.param.deviceType)
    if input_tab.command.param.deviceType  or input_tab.command.param.deviceName then
        if input_tab.command.param.deviceType then
            ret.device = input_tab.command.param.deviceType
        end
        if input_tab.command.param.deviceName then
            ret.device = input_tab.command.param.deviceName
        end
        
        ret.inputText = ori
        
        ret.attribute, ret.action, ret.value = _get_sp_attribute(input_tab)
        print('encode')
        print(cjson.encode(ret))
    end
end

-- -- {"input":"离家模式","command":{"api":"DUI.SmartHome.OpenMode","param":{"mode":"离家模式"}}}
-- local function speech_smarthome_to_hope_smarthome_scene(input_tab, ori)
--     local ret = {
--         catalog = 'smart',
--         ['local'] = 'None',
--     }
-- end


local p_json = cjson.decode(sp_param)
speech_sh_to_hope_sh_device(p_json, p_json.input or "")