#!/usr/bin/lua
local http = require("socket.http")
local ltn12 = require("ltn12")
local json = require("cjson")
local md5 = require("md5")
local lanes = require("lanes").configure()
local socket = require("socket")
-- local htmlparser = require("htmlparser")

local QQ_MUSIC_APPID = "2000000613"
local QQ_MUSIC_APPKEY = "MDBcwRNlvchdGoNr"
local QQ_MUSIC_PACKAGE_NAME = "test.sizheng.com"
local QQ_MUSIC_DEVICE_ID = "88056404100172" -- 这个可能是SN号,是会变化的
local QQ_MUSIC_QRCODE_TYPE = "universal"    --支持QQ音乐/QQ/微信扫码
local QQ_MUSIC_SIGN_VERSION = "v2"
local QQ_MUSIC_DEV_NAME = "hope"            -- 这个可能会变

local qq_music_access_token = 'auhee01f07034c7422d87b2074d1ea520f585955a56ee57afee30c0584ef3548abd'
local qq_music_open_id = '1725025378605956680'

local qq_music_domain_name = 'https://qplaycloud.y.qq.com/rpc_proxy/fcgi-bin/music_open_api.fcg'
local qq_music_cmd_get_songlist_meta = 'fcg_music_custom_get_songlist_meta.fcg'
local qq_music_cmd_get_songlist_detail = 'fcg_music_custom_get_songlist_detail.fcg'
local qq_music_cmd_get_song_info_batch = 'fcg_music_custom_get_song_info_batch.fcg'


local ip_addr = ""
-- local handle = io.popen('curl -s "https://api.ipify.org?format=json"')
-- local result = handle:read("*a")
-- handle:close()

-- -- local json = require("cjson")
-- -- local response_table = json.decode(result)
-- print("Public IP Address: " .. result)

local songlist_dissid_file_path = "/home/mcd/MyWorkSpace/MyGithub_Project/My_C_Test/QQMusic/music_mate/songlist_diss_id/"
local songlist_detail_path = "/home/mcd/MyWorkSpace/MyGithub_Project/My_C_Test/QQMusic/music_mate/songlist_detail/"

function writefile( path, str )
    local f = io.open(path, "w")
    if f then
        -- print("Writing file")
        f:write(str)
        f:close()
    else
        print("Error: Could not write file");
    end
end


-- 获取公网IP
local function get_external_ip()
    local response_body = {}
    local ip_address = ""
    -- local http = require("socket.http")

    local response_body, status_code, headers, status_text = http.request("https://open.nbhope.cn/ip.jsp")

    if status_code == 200 then
        -- print("Response: " .. response_body)

        -- 使用模式匹配提取 IP 地址
        ip_address = response_body:match("<div>%s*(%d+%.%d+%.%d+%.%d+)%s*</div>")

        if ip_address == '' then
            --     print("Public IP Address: " .. ip_address)
            -- else
            print("IP address not found in the response")
        end
    else
        print("Error: " .. status_text .. " (Status code: " .. status_code .. ")")
    end

    return ip_address
end

-- print(get_external_ip())
-- 获取时间戳
local function get_timestamp()
    return os.time()
end
ip_addr = get_external_ip()
print(ip_addr)

-- app_id=2000000613&client_ip=125.116.17.145&cmd=1&device_id=88056404100172&login_type=6&opi_cmd=fcg_music_custom_get_songlist_meta.fcg&qqmusic_access_token=auhee5bf81e55328ef601add4d5435e3941f06e479d7d614b7733e069d3ee5490f1&qqmusic_open_appid=2000000613&qqmusic_open_id=9441910382022780884&sign_version=v2&timestamp=1722330769_MDBcwRNlvchdGoNr
local function table_to_query_string(t)
    local parts = {}
    local keys = {}

    -- 收集所有键
    for k in pairs(t) do
        table.insert(keys, k)
    end

    -- 对键进行升序排序
    table.sort(keys)

    -- 将键值对格式化为 key=value
    for _, k in ipairs(keys) do
        table.insert(parts, k .. "=" .. tostring(t[k]))
    end

    -- 将所有部分连接成一个字符串
    return table.concat(parts, "&")
end

local function get_songlist_send_string(cmd, opi_cmd, category_id, diss_id, song_mid, page, page_size)
    local body = {}

    body.app_id = QQ_MUSIC_APPID
    body.client_ip = ip_addr
    body.device_id = QQ_MUSIC_DEVICE_ID
    body.opi_cmd = opi_cmd
    body.qqmusic_access_token = qq_music_access_token
    body.qqmusic_open_appid = QQ_MUSIC_APPID
    body.qqmusic_open_id = qq_music_open_id
    body.sign_version = QQ_MUSIC_SIGN_VERSION
    body.timestamp = get_timestamp()
    body.login_type = 6 -- 这个参数是需要的
    -- body.size = 50
    -- body.page = 1

    if cmd > 0 then
        body.cmd = cmd
    end
    if cmd == 2 then
        if diss_id >= 0 then
            body.dissid = diss_id
            body.page_size = 30
        end
        if category_id > 0 then
            body.category_id = category_id
        end
    end

    if song_mid ~= "" then
        body.song_mid = song_mid
    end

    local query_string = table_to_query_string(body)

    return query_string
end

-- 获取歌单
local function get_songlist_mate(cmd, category_id)
    -- print(get_songlist_mate())
    local base_string = get_songlist_send_string(cmd, qq_music_cmd_get_songlist_meta, category_id, -1, "")
    local sign_string = base_string .. '_' .. QQ_MUSIC_APPKEY
    -- 计算 MD5 哈希值
    local hash = md5.sumhexa(sign_string)
    -- 打印 MD5 哈希值
    -- print("MD5 Hash: " .. hash)

    -- print(base_string..'&sign='..hash)
    local param_string = base_string .. '&sign=' .. hash
    -- print(param_string)
    local url = qq_music_domain_name .. '?' .. param_string
    local http = require("socket.http")
    local response_body, status_code, status_text = http.request(url)
    if status_code ~= 200 then
        --     print("response_body: " .. response_body)
        -- else
        print("Error: " .. status_text .. " (Status code: " .. status_code .. ")")
    end

    return response_body
end

local function get_songlist_detail(cmd, diss_id)
    local base_string = get_songlist_send_string(cmd, qq_music_cmd_get_songlist_detail, 0, diss_id, "")
    local sign_string = base_string .. '_' .. QQ_MUSIC_APPKEY
    -- 计算 MD5 哈希值
    local md5 = require("md5")
    local hash = md5.sumhexa(sign_string)
    -- 打印 MD5 哈希值
    -- print("MD5 Hash: " .. hash)

    -- print(base_string..'&sign='..hash)
    local param_string = base_string .. '&sign=' .. hash
    -- print(param_string)
    local url = qq_music_domain_name .. '?' .. param_string
    local http = require("socket.http")
    local response_body, status_code, status_text = http.request(url)
    if status_code ~= 200 then
        --     print("response_body: " .. response_body)
        -- else
        print("Error: " .. status_text .. " (Status code: " .. status_code .. ")")
    end

    return response_body
end

local function get_song_info_batch(cmd, song_ids)
    local base_string = get_songlist_send_string(cmd, qq_music_cmd_get_song_info_batch, 0, -1, song_ids)
    local sign_string = base_string .. '_' .. QQ_MUSIC_APPKEY
    -- 计算 MD5 哈希值
    local hash = md5.sumhexa(sign_string

    )
    local sign_string = base_string .. '_' .. QQ_MUSIC_APPKEY
    -- 计算 MD5 哈希值
    local hash = md5.sumhexa(sign_string)
end

local function get_songlist_detail_thread_task(diss_id)
    print(diss_id)
    local detail_body = get_songlist_detail(2, diss_id)
    local json = require("cjson")
    local detail_tab = json.decode(detail_body)
    local detail_num = #detail_tab.song_list
    print('             detail total songs num = ' .. detail_tab.total_num)
end

local function create_songlist_detail_thread(param)
    local lanes1 = require("lanes").configure()
    local thread_gen = lanes1.gen("*", get_songlist_detail_thread_task)
    local thread = thread_gen(param)

    -- 分离线程
    thread:cancel() -- 取消对线程结果的等待，这样就不会阻塞主线程
end

-- 线程任务函数
local function get_songlist_thread_task(param)
    -- print("-----------------------")
    -- local https = require("socket.http")
    local diss_info_num = #param.diss_info
    -- print(diss_info_num)
    if diss_info_num > 0 then
        for i = 1, diss_info_num do
            -- print('         diss[' ..
            --     i ..
            --     '].creator_name = ' ..
            --     param.diss_info[i].creator_name .. ', diss[' .. i .. '].diss_id = ' .. param.diss_info[i].diss_id)
                -- local json = require("cjson")
                -- local str = json.encode(param.diss_info[i])
                local name = param.diss_info[i].creator_name:gsub("/", "")
                local file, err = io.open(songlist_dissid_file_path..name, "w")
                if not file then
                    print("Failed to open file: " .. err)
                end

                file:write(param.diss_info[i].diss_id .. "\n")
                file:close()


                local command = 'mkdir -p "' .. songlist_detail_path ..param.diss_info[i].diss_id.. '"'

                -- 执行命令创建目录
                local result = os.execute(command)
                -- if result then
                --     print("Directory created successfully")
                -- else
                --     print("Failed to create directory: ")
                -- end

            -- local detail_body = get_songlist_detail(2, param.diss_info[i].diss_id)
            -- local json = require("cjson")
            -- local detail_tab = json.decode(detail_body)
            -- local detail_num = #detail_tab.song_list
            -- print('             detail total songs num = ' .. detail_tab.total_num)
        end
    end
end

local function create_songlist_thread(param)
    local thread_gen = lanes.gen("*", get_songlist_thread_task)
    local thread = thread_gen(param)

    -- 分离线程
    thread:cancel() -- 取消对线程结果的等待，这样就不会阻塞主线程
end



local start = get_timestamp()
local response_body = get_songlist_mate(1, 0)
-- print(response_body)
local tab = json.decode(response_body)
if tab.ret ~= 0 then
    return
end
local num = #tab.group
print('tab number = ' .. num)
if num > 0 then
    for i = 1, num do
        -- print('tab[' .. i .. '].name = ' .. tab.group[i].group_name)
        local label_num = #tab.group[i].label
        -- print(' label_num = ' .. label_num)
        for j = 1, label_num do
            -- print('     label[' ..
            --     j ..
            --     '].name = ' .. tab.group[i].label[j].name .. ', label[' .. j .. '].id = ' .. tab.group[i].label[j].id)
            local id = tab.group[i].label[j].id
            -- get_songlist_mate(2, id)
            local song_body = get_songlist_mate(2, id)
            -- print(song_body)
            local diss_tab = json.decode(song_body)
            create_songlist_thread(diss_tab)
        end
    end
end
print("use time: ", get_timestamp() - start)

-- -- 2067204281
-- local detail_body = get_songlist_detail(2, 9202314966)
-- print(detail_body)
-- local songArray = {}
-- local detail_tab = json.decode(detail_body)
-- local num = #detail_tab.song_list

-- -- print('tab number = ' .. num)
-- if num > 0 then
--     for i = 1, num do
--         print('song[' .. i .. '].name = ' .. detail_tab.song_list[i].song_name..', mid = ' .. detail_tab.song_list[i].song_mid)
--         -- table.insert(songArray, detail_tab.song_list[i].song_mid)
--     end
-- end

-- local stringParts = {}
-- -- for i, v in ipairs(songArray) do
-- --     -- print(i, v)
-- --     table.insert(stringParts, v .. "," .. tostring(songArray[i]))
-- -- end
--  -- 将所有部分连接成一个字符串
-- print(table.concat(songArray, ","))



-- unplayable_code有如下可能：

-- 0，无提示；
-- 1，应版权方要求暂不能播放，QQ音乐正在争取中；
-- 2，应版权方要求不能免费播放，可付费后畅享；
-- 4，应版权方要求，暂无法在当前国家或地区提供此歌曲服务；
-- 5，该歌曲暂无音频资源，正努力寻找中；
-- 8，当前接口仅有浏览歌曲信息权限；
-- 11，应版权方要求购买后才能收听，请到手机QQ音乐购买；
-- 12，应版权方要求不能在线播放，可下载后播放；
-- 100，抱歉，该歌曲无法在设备上播放；
-- 110， 该内容需开通超级会员后收听；
-- 播放链接字段
-- 002CLpsa2fO6XM
-- song_play_url

-- 002Mu8Y03KLEjY,002pkGqx2W6WeU
-- try_30s_url


-- 加载 Lua Lanes 库

-- -- 子线程任务函数
-- local function thread_task(url)
--     -- local socket = require("socket")
--     for i = 1, 5 do
--         local http = require("socket.http")

--         local body, code, headers, status = http.request(url)

--         if not body then
--             return "Failed to fetch URL: " .. tostring(code)
--         end
--         print(status)
--     end

--     -- return body, code, headers, status
-- end

-- -- 创建并启动分离式子线程
-- local function create_detached_thread(url)
--     local thread_gen = lanes.gen("*", thread_task)
--     local thread = thread_gen(url)

--     -- 分离线程：不等待结果，不检查状态
--     thread:cancel()
-- end

-- -- 主线程中的任务
-- print("Hello from main")

-- -- 创建并启动一个分离式子线程
-- for i = 1, 5 do
--     create_detached_thread("http://www.google.com")
-- end
-- -- create_detached_thread("http://www.baidu.com")
-- -- create_detached_thread("http://www.github.com")

-- -- 主线程继续执行
-- print("Main thread finished")

-- -- 为了让分离线程有机会执行，可以添加一个短暂的延时
socket.sleep(30)
print("finished")
