
local json = require("cjson")


local path = '80年代港乐丨“三王一后”的传奇'
local str  = '12431dsafw'
local f = io.open(path, "w+")
if f then
    f:write(str)
    f:close()
else
    print('can not write file: '..path)
end