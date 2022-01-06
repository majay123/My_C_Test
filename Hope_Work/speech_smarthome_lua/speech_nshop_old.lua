#!/usr/bin/lua

-- package.path = '/mnt/xfont/lua/?.lua'
-- package.path = '/mnt/xfont/lua/?.lua'
-- local cjson = require("cjson.safe")
local json = require("cjson.safe")
-- require("villa_common")
-- require("utils")
-- require("http_regist")


-- local dev_path = '/mnt/xfont/http_dev.json'
-- local scene_path = '/mnt/xfont/scenes_list.json'

local MSG_AI_TO_MQTT_CUSTOM_GW_CONTROL = 15011

--[[
    由于hope的格式固定, 而且传输结构与speech返回的json结构更加接近, 所以能够更加简单的做出映射表; 
    halo则更加随意模糊, 结构也更加复杂, 而且经常有定制需求.

    speech -> hope 键值映射表
    hope_attribute: 
        键 -> speech 返回json参数的键
        键.name -> 对应hope平台相应属性的名字,
        键.value -> 相应键的特殊值, 除数字/最值之外的特定值
        键.act -> 相应键的特殊动作值, 数字/最值/加减 使用默认的 TO/PLUS/MINUS
]]
local hope_attribute = {
    ['操作'] = {
        name = 'STATE',
        act = {
            ['开'] = 'ON',
            ['关'] = 'OFF',
            ['停'] = 'STOP',
            ['切换'] = 'TOGGLE',
        },
    },
    ['风速'] = {
        name = 'WIND',
        value = {
            ['高'] = 'HIGH',
            ['中'] = 'MEDIUM',
            ['低'] = 'LOW',
            ['自动'] = 'AUTO',
            ['+'] = 'HIGH',
            ['max'] = 'HIGH',
            ['-'] = 'LOW',
            ['min'] = 'LOW',
        },
    },
    ['频道'] = {
        name = 'CHANNEL',
    },
    ['温度'] = {
        name = 'TEMPERATURE',
    },
    ['音量'] = {
        name = 'VOLUME',
        act = {
            ['静音'] = 'MUTE',
            ['取消静音'] = 'CMUTE',
        },
    },
    ['亮度'] = {
        name = 'LIGHT',
    },
    ['饱和度'] = {
        name = 'SATURATION',
    },
    ['色彩'] = {
        name = 'HUE',
    },
    ['位置'] = {
        name = 'POSITION',
    },
    ['进度'] = {
        name = 'POSITION',
    },
    ['模式'] = {
        name = 'MODE',
        act = {
            ['制冷'] = 'COLD',
            ['制热'] = 'HEAT',
            ['通风'] = 'WIND',
            ['干燥'] = 'DRY',
            ['除湿'] = 'DRY',
        }
    }
}

-- bst cstmer dt spt t sce!!
-- T B Slst hr.
local BAN = true
local hope_sce_ban = {
    ['通风'] = BAN,
    ['制冷'] = BAN,
    ['制热'] = BAN,
    ['干燥'] = BAN,
    ['除湿'] = BAN,
}

--[[
    hope_attribute: 相反的映射, 从hope的json格式转成语音文字
]]
local hope_tts = {
    STATE = {
        voice = '',
        act = {
            ON	 = '打开',
            OFF	 = '关闭',
            STOP = '停止',
            TOGGLE = '切换',
        },
    },
    WIND = {
        voice="风速",
        val = {
            HIGH = '高档',
            MEDIUM = '中档',
            LOW = '低档',
            AUTO = '自动',
        }
    },
    CHANNEL = {
        voice="频道",
        act = {
            default = '切到',
            PLUS = '下一个频道',
            MINUS = '上一个频道',
        }
    },
    TEMPERATURE = {
        voice="温度",
        val = {
            MAX = '最高',
            MIN = '最低',
        },
    },
    VOLUME = {
        voice="音量",
        act = {
            MUTE = '静音',
            CMUTE = '恢复音量',
        }
    },
    LIGHT = {
        voice="亮度"
    },
    SATURATION = {
        voice="饱和度"
    },
    HUE = {
        voice="色彩"
    },
    POSITION = {
        voice="位置"
    },
    MODE = {
        voice='模式',
        act = {
            default = '切换为',
            COLD = '制冷模式',
            HEAT = '制热模式',
            WIND = '通风模式',
            DRY = '除湿模式'
        },
    },
}

local hope_default_act = 'TO'
local hope_act = {
    ['+'] = 'PLUS',
    ['-'] = 'MINUS',
}
local hope_tts_act = {
    TO = '设置为',
    MINUS = '降低',
    PLUS = '提高',
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
    else
        return str, 0
    end
end



local function _get_attribute( param )
    for k, v in pairs(hope_attribute) do
        if param[k] then
            if v.value then
                for _k, _v in pairs(v.value) do
                    if param[k]:find(_k) then
                        return v.name , hope_default_act, _v
                    end
                end
            end
            if v.act then
                for _k, _v in pairs(v.act) do
                    if param[k]:find(_k) then
                        return v.name , _v, nil
                    end
                end
            end
            for _k, _v in pairs(hope_act) do
                if param[k]:find(_k) then
                    local s, e = string.find(param[k], '%d+')
                    local num = (s and string.sub(param[k], s, e)) or nil
                    -- return v.name, _v, nil
                    return v.name, _v, num
                end
            end
            for _k, _v in pairs(hope_value_to) do
                if param[k]:find(_k) then
                    return v.name, hope_default_act, _v
                end
            end
            local s, e = string.find(tostring(param[k]), '%d')
            -- in case of value with '%'
            if s then
                -- return v.name, hope_default_act, tostring(param[k])
                
                -- deal with server cannot support %for now!
                local nu, per = chk_num_persent1(param[k])
                return v.name, hope_default_act, tostring(nu)
            end
        end
    end
end

local function _tab_tts( tab )
    -- local tts = '马上为您'

    -- --[[还原设备名称, 主要还原设备编号到设备名称]]
    -- local name = ''
    -- local a, b = string.find(tab.device, '%d+')
    -- if a then
    --     name = string.sub(tab.device, 1, a-1)
    --     local n = string.sub(tab.device, a, b)
    --     name = name ..utils.num2chs(n)
    -- else
    --     name = tab.device
    -- end

    -- local att = hope_tts[tab.attribute]
    -- if att then
    --     --[[属性有动作值]]
    --     if att.act then
    --         if att.act[tab.action] then
    --             return tts..'把'..name..(att.act.default or '')..att.act[tab.action]
    --         end
    --     end

    --     --[[属性值]]
    --     local val_tts =''
    --     local n = tonumber(tab.value)
    --     if n then 
    --         val_tts = utils.num2chs(n)
    --     else
    --         local nu, per = utils.chk_num_persent(tab.value)
    --         if per == 2 then
    --             val_tts = '百分之'..utils.num2chs(nu)
    --         else
    --             val_tts = att.val and att.val[tab.value] or ''
    --         end
    --     end

    --     return tts..'将'..name..'的'..att.voice..hope_tts_act[tab.action]..val_tts
    -- end
    -- return ''
end


local test = '{"command":{"param":{"亮度":"+","设备名称":"阅读灯"},"api":"dev_ctrl"}}'
local item = json.decode(test)

print(item.command.api)
local r1, r2, r3 = _get_attribute(item.command.param)
print(r1, r2, r3)