local json = require("cjson.safe")

local json_str = [[
{
    "group_name": "语种",
    "label": [
        {"name": "英语", "id": 3},
        {"name": "粤语", "id": 146},
        {"name": "韩语", "id": 4},
        {"name": "日语", "id": 5},
        {"name": "国语", "id": 1},
        {"name": "闽南语", "id": 113},
        {"name": "小语种", "id": 6},
        {"name": "法语", "id": 149},
        {"name": "拉丁语", "id": 3205}
    ]
}
]]

-- 解析为 Lua 表
local data = json.decode(json_str)

-- 需要保留的 name 列表
local target_names = {
    ["英语"] = true,
    ["粤语"] = true,
    ["韩语"] = true,
    ["日语"] = true,
    ["国语"] = true,
    ["闽南语"] = true
}

-- 构建新的表
local result = {}

for _, item in ipairs(data.label) do
    if target_names[item.name] then
        table.insert(result, item)
    end
end

-- 转换回 JSON 字符串输出
local output = json.encode(result)
print(output)