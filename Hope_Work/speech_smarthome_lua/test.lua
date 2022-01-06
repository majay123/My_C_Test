#!/usr/bin/lua

local h = '[%z\1-\127\194-\244][\128-\191]*'
local  scene = '情景五:我玩游戏了:情景5'
-- scene:find('^:')
-- print(scene:len())
-- print(scene:find('(%a+)%s*=%s*(%a+)'))
-- print(scene:match('%a+:%a+:%a+'))

-- print(scene:match(':+'))
-- print(scene:sub(scene:find(':+') + 1, scene:len()))
-- local scene1 = scene:sub(scene:find(':') + 1, scene:len())
-- print(scene1:sub(scene1:find(':') + 1, scene1:len()))


while scene:find(':') do
    scene = scene:sub(scene:find(':+') + 1, scene:len())
end
print(scene)


-- pair = " name = Anna "
print(string.find(scene, "([%z\1-\127\194-\244][\128-\191]*)"))
-- print(string.find(scene, "%n:"))
s = "Deadline is 30/05/1999, firm"
date = "%d+/%d+/%d+"
-- date = "%d%d/%d%d/%d%d%d%d"
print(string.sub(s, string.find(s, date)))    --> 30/05/1999


