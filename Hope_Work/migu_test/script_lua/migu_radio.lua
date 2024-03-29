#!/usr/bin/lua
package.path = '/tmp/lua/?.lua'
require("utils")
local json = require("cjson.safe")
local migu = hpio.migu

local RADIO_PATH       = "/tmp/music/music_radio/radio_list/"
local RADIO_MUSIC_PATH       = "/tmp/music/music_radio/radio_music/"

-- 替换特殊符号


local function  get_musicinfos()

end

local function get_radio_info(columnId, radioinfo)
    local file_name = utils.replace_symbol(radioinfo.radioId, '%p(.-)', '')
    utils.writefile(RADIO_PATH..columnId..'/'..file_name, json.encode(radioinfo))
end


function get_migu_radio()
    local file=io.open(RADIO_PATH)
    if  not file then
        os.execute('mkdir -p '..RADIO_PATH)
    else
        file:close()
    end

    local file1=io.open(RADIO_MUSIC_PATH)
    if  not file1 then
        os.execute('mkdir -p '..RADIO_MUSIC_PATH)
    else
        file1:close()
    end
    
    local info = {}
    local radio_info = migu.migu_get_radio_info()
    -- print("radio: ", radio_info)
    local tab = json.decode(radio_info)
    local columnNum = #tab
    print("columnNum: ", columnNum)
    for i=1, columnNum do
        -- print("columnInfo ", tab[i].columnName, tab[i].columnId)
        info.columnId = tab[i].columnId
        info.columnName = tab[i].columnName
        local str = json.encode(info)
        os.execute('mkdir -p '..RADIO_PATH..tab[i].columnId)
        local file_name = utils.replace_symbol(tab[i].columnName, '%p(.-)', '')
        utils.writefile(RADIO_PATH..file_name, str)

        local radioNum = #tab[i].radios
        print("radioNum: ", radioNum)
        for j=1, radioNum do
            get_radio_info(tab[i].columnId, tab[i].radios[j])
        end
    end
    return 0
end