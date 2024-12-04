#!/usr/bin/lua
local http = require("socket.http")
local ltn12 = require("ltn12")
local json = require("cjson")
local md5 = require("md5")
local lanes = require("lanes").configure()
local socket = require("socket")
local socket_url = require("socket.url")

local ip_addr = ""

local function get_random_num(min, max)
    -- 设置随机种子（可选）
    math.randomseed(os.time())

    -- 生成 1 到 4 之间的随机数
    local randomNumber = math.random(min, max)

    -- print("Random number between 1 and 4: " .. randomNumber)

    return randomNumber
end

function writefile( path, str )
    local f = io.open(path, "w+")
    if f then
        f:write(str)
        f:close()
    end
end

-- 获取公网IP
local function get_external_ip()
    local response_body = {}
    local ip_address = {}
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

-- print(type(get_external_ip()))
-- print(type(ip_addr))

-- ip_addr = get_external_ip()
-- print(ip_addr)

local songlist_dissid_file_path = "/home/mcd/MyWorkSpace/MyGithub_Project/My_C_Test/QQMusic/music_mate/songlist_diss_id/"

function writefile(path, str)
    local f = io.open(path, "w+")
    if f then
        f:write(str)
        f:close()
    else
        print("error writing")
    end
end

local client_ip = ''
local city_code = ''
function hope_get_clientIp()
    local str, err = http.request('https://open.nbhope.cn:443/contents/network/info')

    -- print(str)
    local tab = json.decode(str)
    client_ip = tab.object.clientIp

    -- print(client_ip)
    -- print(#clientIp)
    return client_ip
end

function hope_get_local()
    local str, err = http.request('https://open.nbhope.cn:443/contents/place/info')

    -- print(str)
    local tab = json.decode(str)
    city_code = tab.object.cityCode
    -- print(tab.object.cityCode)
    -- print(#tab.object.cityCode)
end

-- writefile(songlist_dissid_file_path .. "songlist_diss_id.txt", "1234567890")


function hope_get_weather_from_hope_server(server)
    hope_get_clientIp()
    hope_get_local()
    print(client_ip)
    print(city_code)
end

-- hope_get_weather_from_hope_server(）

local function qqmusic_data_ascending_order(params)
    -- 获取所有键
    local keys = {}
    for k in pairs(params) do
        table.insert(keys, k)
    end

    -- 对键进行升序排序
    table.sort(keys)

    -- 构造排序后的参数字符串
    local sorted_params = {}
    for _, k in ipairs(keys) do
        table.insert(sorted_params, k .. "=" .. params[k])
    end

    -- 将表转换为以 & 分隔的字符串
    local result = table.concat(sorted_params, "&")

    return result
end

function qqmusic_api_get_qrcode(url, app_id, device_id, app_key)
    local params = {}
    local auth = {
        response_type = 'code',
        state = '123456'
    }

    local client_ip = hope_get_clientIp()

    params.app_id = app_id
    params.qqmusic_open_appid = app_id
    params.client_ip = client_ip
    params.device_id = device_id
    params.opi_cmd = 'fcg_music_custom_sdk_get_qr_code.fcg'
    -- params.qqmusic_package_name = qqmusic_package_name
    params.sign_version = 'v2'
    params.qqmusic_encrypt_auth = json.encode(auth)
    params.qqmusic_qrcode_type = 'universal'
    params.qqmusic_dev_name = 'hope'
    params.timestamp = 1728539742--os.time()

    
    local result = qqmusic_data_ascending_order(params)
    print(result)

    -- 计算 MD5 值
    local hash = md5.sumhexa(result..'_'..app_key)
    -- 输出结果
    print("MD5 Hash: " .. hash)

    params.qqmusic_encrypt_auth = socket_url.escape(json.encode(auth))

    result = qqmusic_data_ascending_order(params)

    local data = result..'&sign=' .. hash
    -- print(data)

    local httpdata = url..data 

    local str, err = http.request(httpdata)

    -- 输出请求结果
    -- print("Status Code:", err)
    -- print("Response Body:", str)
    -- {"msg":"ok","ret":0,"traceId":"5fe10faed0c290fb","wx_qrcode_image":"","sub_ret":0,"vcheck":0,"sdk_qr_code":"https://developer.y.qq.com/openid/1/?a=2000000627&c=auh-O8-jcwhb8YCxRzNL65Fu","auth_code":"auh-O8-jcwhb8YCxRzNL65Fu","ts":1727663083335}

    local qr_data = json.decode(str)
    
    if qr_data.msg and (qr_data.msg == 'ok') and qr_data.auth_code then
        -- print(qr_data.sdk_qr_code)
        -- print(qr_data.auth_code)
        return qr_data.sdk_qr_code, qr_data.auth_code
    else
        print('msg error')
    end

    return nil
end


-- params.app_id = '2000002621'
-- params.client_ip = client_ip
-- params.qqmusic_open_appid = '2000002621'
-- params.device_id = '1664745890332315648'
-- -- params.qqmusic_encrypt_auth = '%7B%22response_type%22%3A%22code%22%2C%22state%22%3A%22123456%22%7D'
-- params.qqmusic_encrypt_auth = json.encode(auth)
-- params.qqmusic_qrcode_type = 'universal'
-- params.opi_cmd = 'fcg_music_custom_sdk_get_qr_code.fcg'
-- params.timestamp = os.time()
-- -- params.timestamp = 1727601070
-- params.qqmusic_dev_name = 'hope'
-- params.qqmusic_package_name = 'test.szhkeji.com'
-- params.sign_version = 'v2'
-- local qr, code = qqmusic_api_get_qrcode('https://test.szhkeji.com/rpc_proxy/music_open_api?', '2000002621','1664745890332315648',  'NdeQijOnksQumgfC')
-- print(qr, code)


-- local http_time = 'http://api.pinduoduo.com/api/server/_stm'
-- local str, err = http.request(http_time)
-- print("Status Code:", err)
-- print("Response Body:", str)


function qqmusic_api_check_scan_response(url, app_id, device_id, opi_cmd, auth_code, sign_version, app_key)
    local params = {}
    local auth = {
        response_type = 'code',
        state = '123456'
    }

    local client_ip = hope_get_clientIp()

    params.app_id = app_id
    params.qqmusic_openid_appId = app_id
    params.client_ip = client_ip
    params.device_id = device_id
    params.opi_cmd = opi_cmd
    params.qqmusic_openid_authCode = auth_code
    params.sign_version = sign_version
    params.timestamp = os.time()

    local result = qqmusic_data_ascending_order(params)

    -- 计算 MD5 值
    local hash = md5.sumhexa(result..'_'..app_key)
    -- 输出结果
    print("MD5 Hash: " .. hash)

    local data = result..'&sign=' .. hash

    local httpdata = url..data 

    local str, err = http.request(httpdata)

    print(str)
    local res_data = json.decode(str)

    if res_data.msg and (res_data.msg == 'ok') and res_data.encryptString then
        return res_data.encryptString
    else
        return nil
    end
end



local songlist_path = '/home/mcd/MyWorkSpace/MyGithub_Project/My_C_Test/QQMusic/music_mate/songlist_diss_id/'
local songlist_detail = '/home/mcd/MyWorkSpace/MyGithub_Project/My_C_Test/QQMusic/music_mate/songlist_detail/'

-- 获取排行榜榜单详情
-- fcg_music_custom_get_toplist_info.fcg
function qqmusic_api_get_toplist_info(index,url, app_id, app_key, device_id, open_id, access_token, top_id, page_size, page)
    local params = {}
    local client_ip = hope_get_clientIp()

    params.opi_cmd = 'fcg_music_custom_get_toplist_info.fcg'
    params.app_id = app_id
    params.qqmusic_openid_appId = app_id
    params.device_id = device_id
    -- params.qqmusic_openid_authCode = auth_code
    -- params.sign_version = 'v2'
    params.client_ip = client_ip
    params.timestamp = os.time()
    params.qqmusic_open_id = open_id
    params.qqmusic_access_token = access_token
    params.login_type = 6
    params.top_id = top_id
    if page_size then
        params.page_size = page_size
    else
        params.page_size = 20
    end

    params.page = page

    local result = qqmusic_data_ascending_order(params)

    -- 计算 MD5 值
    local hash = md5.sumhexa(result..'_'..app_key)
    -- 输出结果
    print("MD5 Hash: " .. hash)

    local data = result..'&sign=' .. hash

    local httpdata = url..data 

    -- print(httpdata)

    local str, err = http.request(httpdata)

    -- print(str)
    local res_data = json.decode(str)

    if res_data.msg and (res_data.msg == 'ok')then
        if res_data.songlist then
            print(#res_data.songlist)
            for i, song in ipairs(res_data.songlist) do
                -- {"musicName":"American Oxygen","musicId":"6005660A4HT"}
                local song_list = {}
                song_list.musicName = song.song_name
                song_list.musicId = song.song_mid
                -- print(i, song.song_name, song.song_mid)
                local song_str = json.encode(song_list)
                writefile(songlist_detail..index..'/'..song.song_name, song_str)
            end
        end
        return 1
    else
        return 0
    end
end

-- 将toplist转换成json，并保存
local function save_toplist_to_file(index, top_id, top_name)
    local list_tab = {}
    
    list_tab.index = index
    list_tab.top_id = top_id
    list_tab.name = top_name
    list_tab.pic_name = string.format('/mnt/xfont/music/pic/pic_gedan2.png', get_random_num(1, 4))
    -- list_tab.pic_name = '/mnt/xfont/music/pic/pic_gedan'..get_random_num(1, 4)..'.png'
    local top_str = json.encode(list_tab)
    -- print(top_str)
    writefile(songlist_path..index, top_str)
    os.execute('mkdir -p '..songlist_detail..index)
    --[[ QQMusic/music_mate/songlist_diss_id ]]
    -- qqmusic_api_get_toplist_info()
end

-- 获取排行榜榜单
-- 获取排行榜，保存到文件
function qqmusic_api_get_toplist(url, app_id, opi_cmd, app_key, device_id, open_id, access_token)
    local params = {}
    local client_ip = hope_get_clientIp()
    local count = 0

    params.opi_cmd = opi_cmd
    params.app_id = app_id
    params.qqmusic_openid_appId = app_id
    params.device_id = device_id
    -- params.qqmusic_openid_authCode = auth_code
    -- params.sign_version = 'v2'
    params.client_ip = client_ip
    params.timestamp = os.time()
    params.qqmusic_open_id = open_id
    params.qqmusic_access_token = access_token
    params.login_type = 6

    local result = qqmusic_data_ascending_order(params)

    -- 计算 MD5 值
    local hash = md5.sumhexa(result..'_'..app_key)
    -- 输出结果
    print("MD5 Hash: " .. hash)

    local data = result..'&sign=' .. hash

    local httpdata = url..data 

    -- print(httpdata)

    local str, err = http.request(httpdata)

    -- print(str)
    local res_data = json.decode(str)

    if res_data.msg and (res_data.msg == 'ok')then
        if res_data.group_list then
            -- print(#res_data.group_list)
            group_list = res_data.group_list
            for i, group in ipairs(group_list) do
                -- print(i, group.group_name)
                if group.group_top_list then
                    -- print(#group.group_top_list)
                    for j, top_list in ipairs(group.group_top_list) do
                        count = count + 1;
                        -- print('  ', j, top_list.top_name, top_list.top_id)
                        save_toplist_to_file(count, top_list.top_id, top_list.top_name)
                        -- qqmusic_api_get_toplist_info(count, url, app_id, app_key, device_id, open_id, access_token, top_list.top_id, 30 , 0)
                    end
                end
            end
            print('count = ' .. count)
        end
        return 1
    else
        return 0
    end
end
-- authcode: 889330ACE8E248E6900D3DDAECCD5002
-- devicd: 1664745890332315648
-- code: code-O8Ur6ZzTE0E9lxo7y84
-- token: auhd920dd2c73f198952ae626a8cb3ba134acfcedb62ee545506c42b2ccd908cde6
-- qqmusic_open_id: 2562110644155510541
-- 没扫描{"ret":-10,"msg":"authCode is expire","traceId":"0abcc810aca3fa5c","sub_ret":0,"vcheck":0,"ts":1727687189010}
-- 扫描登陆成功{"ret":0,"msg":"OK","traceId":"5ed5e078ca85a14e","encryptString":"code-O81BHEKPmpHOBim6y7Q","sub_ret":0,"vcheck":0,"ts":1727687373787}
-- qqmusic_api_check_scan_response('https://test.szhkeji.com/rpc_proxy/music_open_api?', '2000002621','1664745890332315648', 'fcg_music_custom_qrcode_auth_poll.fcg', 'auh-O81B5TIOhF6cTi6w0XIu', 'v2', 'NdeQijOnksQumgfC')
-- qqmusic_api_get_toplist('https://test.szhkeji.com/rpc_proxy/music_open_api?', '2000002621', 'fcg_music_custom_get_toplist.fcg', 'NdeQijOnksQumgfC', '1664745890332315648', '2562110644155510541', 'auh3e9d06aa5e6782c54e6e5cec51c62345cea84d32bc4077d1f0f4583d213e8aeb')
-- qqmusic_api_get_toplist_info('https://test.szhkeji.com/rpc_proxy/music_open_api?', '2000002621', 'fcg_music_custom_get_toplist_info.fcg', 'NdeQijOnksQumgfC', '1664745890332315648', '2562110644155510541', 'auhd920dd2c73f198952ae626a8cb3ba134acfcedb62ee545506c42b2ccd908cde6',62, 50, 0)

-- hope_get_clientIp()
-- print(os.time())

-- print(get_random_num(1, 4))
print(string.format('/mnt/xfont/music/pic/pic_gedan%d.png', get_random_num(1, 4)))