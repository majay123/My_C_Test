#!/usr/bin/lua
package.path = '/tmp/lua/?.lua'
require("utils")
local json = require("cjson.safe")
local migu = hpio.migu

local RANK_PATH       = "/tmp/music/music_rank/rank_list/"
local RANK_MUSIC_PATH       = "/tmp/music/music_rank/rank_music/"

-- 替换特殊符号


local function  get_musicinfos()

end

local function get_rank_info(columnId, rankinfo)
    local file_name = utils.replace_symbol(rankinfo.radioId, '%p(.-)', '')
    utils.writefile(RANK_PATH..columnId..'/'..file_name, json.encode(rankinfo))
end


function get_migu_rank()
    local file=io.open(RANK_PATH)
    if  not file then
        os.execute('mkdir -p '..RANK_PATH)
    else
        file:close()
    end

    local file1=io.open(RANK_MUSIC_PATH)
    if  not file1 then
        os.execute('mkdir -p '..RANK_MUSIC_PATH)
    else
        file1:close()
    end
    
    local info = {}
    local radio_info = migu.migu_get_music_rank()
    -- print("radio: ", radio_info)
    local tab = json.decode(radio_info)
    local columnNum = #tab
    print("columnNum: ", columnNum)
    for i=1, columnNum do
        local songNum = #tab[i].songIds
        if songNum > 0 then
            print("columnInfo ", tab[i].columnName)
            utils.writefile(RANK_PATH..utils.replace_symbol(tab[i].columnName, '%p(.-)', ''), json.encode(tab[i]))
        --     print(utils.readfile(RANK_PATH..utils.replace_symbol(tab[i].columnName, '%p(.-)', '')))
        end
    end
    return 0
end