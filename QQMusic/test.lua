#!/usr/bin/lua
local http = require("socket.http")
local ltn12 = require("ltn12")
local json = require("cjson")
local md5 = require("md5")
local lanes = require("lanes").configure()
local socket = require("socket")

local ip_addr = ""

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

writefile(songlist_dissid_file_path .. "songlist_diss_id.txt", "1234567890")
