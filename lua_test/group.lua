
package.path = '/mnt/xfont/lua/?.lua'
local json = require("cjson.safe")

local json_buf = '{"traceId":"19202649a477a2e8","ret":0,"msg":"success","sub_ret":0,"vcheck":0,"ts":1741914827161,"group":[{"group_name":"推荐","label":[{"name":"推荐","id":100823}]},{"group_name":"热门","label":[{"name":"官方歌单","id":3317},{"name":"私藏","id":3417}]},{"group_name":"主题","label":[{"name":"音乐人在听","id":1069},{"name":"KTV金曲","id":64},{"name":"Chill Vibes","id":3902},{"name":"网络歌曲","id":3056},{"name":"现场音乐","id":95},{"name":"背景音乐","id":107},{"name":"经典老歌","id":59},{"name":"情歌","id":71},{"name":"儿歌","id":3200},{"name":"ACG","id":3202},{"name":"影视","id":3201},{"name":"综艺","id":3256},{"name":"游戏","id":73},{"name":"乐器","id":3204},{"name":"城市","id":93},{"name":"戏曲","id":3361},{"name":"DJ神曲","id":3357},{"name":"MC喊麦","id":60},{"name":"佛教音乐","id":3401},{"name":"厂牌专区","id":3320},{"name":"人气音乐节","id":136},{"name":"精品","id":3819},{"name":"互动歌单","id":3903}]},{"group_name":"场景","label":[{"name":"夜店","id":32},{"name":"学习工作","id":3248},{"name":"咖啡馆","id":3215},{"name":"运动","id":132},{"name":"睡前","id":27},{"name":"旅行","id":36},{"name":"跳舞","id":3216},{"name":"派对","id":33},{"name":"婚礼","id":3214},{"name":"约会","id":35},{"name":"校园","id":39},{"name":"驾驶","id":3901}]},{"group_name":"心情","label":[{"name":"伤感","id":74},{"name":"快乐","id":3142},{"name":"安静","id":13},{"name":"励志","id":16},{"name":"治愈","id":7},{"name":"思念","id":10},{"name":"甜蜜","id":20},{"name":"寂寞","id":9},{"name":"宣泄","id":17}]},{"group_name":"年代","label":[{"name":"00年代","id":144},{"name":"90年代","id":142},{"name":"80年代","id":141},{"name":"70年代","id":140}]},{"group_name":"流派","label":[{"name":"流行","id":3152},{"name":"电子","id":45},{"name":"轻音乐","id":49},{"name":"民谣","id":48},{"name":"说唱","id":42},{"name":"摇滚","id":41},{"name":"爵士","id":46},{"name":"R&B","id":43},{"name":"布鲁斯","id":51},{"name":"古典","id":47},{"name":"后摇","id":3217},{"name":"古风","id":61},{"name":"中国风","id":68},{"name":"乡村","id":44},{"name":"金属","id":3065},{"name":"新世纪","id":53},{"name":"世界音乐","id":3066},{"name":"中国传统","id":3176}]},{"group_name":"语种","label":[{"name":"英语","id":3},{"name":"粤语","id":146},{"name":"韩语","id":4},{"name":"日语","id":5},{"name":"国语","id":1},{"name":"闽南语","id":113},{"name":"小语种","id":6},{"name":"法语","id":149},{"name":"拉丁语","id":3205}]}]}'


local function replace_symbol(input, sy, rp_sy)
    local str = string.gsub(input, sy, rp_sy)
    return str
end

function group_test(glist)
    print(#glist)
    for i, label in ipairs(glist) do
        -- print(label.name, " ", label.id)
    end
end

function test(json_data)
    -- print(json_data)
    local res_data = json.decode(json_data)

    if res_data.msg and ((string.lower(res_data.msg) == string.lower('success')))then
        print(#res_data.group)
        for i, group in ipairs(res_data.group) do
            local ginfo = {}
            ginfo.id = res_data.traceId..tostring(i)
            ginfo.name = replace_symbol(group.group_name, '%p(.-)', '')
            ginfo.list = group.label
            -- print('group list:', #ginfo.list)
            local str = json.encode(ginfo)
            -- print(str)
            -- 反向测试
            local group_data = json.decode(str)
            
            group_test(group_data.list)

        end
    end
end
-- test(json_buf)

function get_songlist(json_data)
    local res_data = json.decode(json_data)

    -- print(#res_data.diss_info)
    if res_data.msg and ((string.lower(res_data.msg) == string.lower('success')))then
        for i, dissinfo in ipairs(res_data.diss_info) do
            local dinfo = {}
            dinfo.id = dissinfo.diss_id
            dinfo.name = replace_symbol(dissinfo.diss_name, '%p(.-)', '')
            local str = json.encode(dinfo)
            print(str)
        end
    end

end
-- get_songlist(json_buf1)
function song_info_parse()
    local file = io.open("song_info.json", "r")  -- 以只读模式打开文件
    if file then
        local content = file:read("*a")    -- 读取整个文件内容
        file:close()                       -- 关闭文件
        -- print(content)
        local res_data = json.decode(content)
        if res_data.msg and ((string.lower(res_data.msg) == string.lower('ok')))then
            if res_data.songlist then
                print(#res_data.songlist)
                for i, song in ipairs(res_data.songlist) do
                    local song_list = {}
                    song_list.id = i
                    song_list.musicName = replace_symbol(song.song_title, '%p(.-)', '')
                    song_list.musicId = song.song_mid
                    -- print(i, song.song_name, song.song_mid)
                    local song_str = json.encode(song_list)
                    print(song_str)
                    -- print(songlist_detail..index..'/'..song.song_name)
                
                end
            end
        end
    else
        print("文件打开失败")
    end
end

function diss_info_parse()
    local file = io.open("diss_info.json", "r")  -- 以只读模式打开文件
    if file then
        local content = file:read("*a")    -- 读取整个文件内容
        file:close()                       -- 关闭文件
        -- print(content)
        local res_data = json.decode(content)
        if res_data.msg and ((string.lower(res_data.msg) == string.lower('success')))then
            if res_data.diss_info then
                print(#res_data.diss_info)
                for i, diss in ipairs(res_data.diss_info) do
                    local diss_list = {}
                    diss_list.index = i
                    diss_list.id = diss.diss_id
                    diss_list.dissName = replace_symbol(diss.diss_name, '%p(.-)', '')
                    -- print(i, song.song_name, song.song_mid)
                    local diss_str = json.encode(diss_list)
                    print(diss_str)
                
                end
            end
        end
    else
        print("文件打开失败")
    end
end

diss_info_parse()