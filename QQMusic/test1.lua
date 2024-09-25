local lfs = require("lfs")

local path = "/home/mcd/MyWorkSpace/MyGithub_Project/My_C_Test/QQMusic/music_mate/songlist_detail/821696356"

-- 使用模式匹配提取最后的数字部分
local id = path:match("(%d+)$")

-- 打印提取的数字
print(id)

-- -- 定义文件路径和要写入的内容
-- local filepath = "file.txt"
-- local content = "This is the new content of the file."
-- local content1 = "other content"

-- -- 打开文件以覆盖模式写入
-- local file, err = io.open(filepath, "w+")
-- if not file then
--     print("Failed to open file: " .. err)
--     return
-- end

-- -- 写入内容到文件
-- local success, write_err = file:write(content)
-- if not success then
--     print("Failed to write to file: " .. write_err)
--     file:close()
--     return
-- end

-- -- 关闭文件
-- local close_success, close_err = file:close()
-- if not close_success then
--     print("Failed to close file: " .. close_err)
--     return
-- end

-- print("File written successfully")



-- -- 打开文件以覆盖模式写入
-- local file, err = io.open(filepath, "w")
-- if not file then
--     print("Failed to open file: " .. err)
--     return
-- end

-- -- 写入内容到文件
-- local success, write_err = file:write(content1)
-- if not success then
--     print("Failed to write to file: " .. write_err)
--     file:close()
--     return
-- end

-- -- 关闭文件
-- local close_success, close_err = file:close()
-- if not close_success then
--     print("Failed to close file: " .. close_err)
--     return
-- end

-- print("File written successfully")

-- local input_string = "Forever //"

-- -- 使用 string.gsub 函数替换所有单斜杠
-- local output_string = input_string:gsub("/", "")

-- -- 打印结果
-- print(output_string)

local songlist_detail_path = "/home/mcd/MyWorkSpace/MyGithub_Project/My_C_Test/QQMusic/music_mate/songlist_detail/"


-- 列出指定目录下的所有子目录
local function list_directories(directory)
    -- 打印目录中的子目录
    for file in lfs.dir(directory) do
        -- 忽略 "." 和 ".."
        if file ~= "." and file ~= ".." then
            local file_path = directory  .. file
            local attr = lfs.attributes(file_path)
            if attr and attr.mode == "directory" then
                print(file_path)
            end
        end
    end
end


-- 计算指定目录下的所有子目录个数
local function count_directories(directory)
    local dir_count = 0

    -- 遍历目录
    for file in lfs.dir(directory) do
        -- 忽略 "." 和 ".."
        if file ~= "." and file ~= ".." then
            local file_path = directory .. "/" .. file
            local attr = lfs.attributes(file_path)
            if attr and attr.mode == "directory" then
                dir_count = dir_count + 1
            end
        end
    end

    return dir_count
end

-- 指定目录路径
-- local dir_path = "/path/to/your/directory"

-- 获取文件个数
-- local num_files = list_directories(songlist_detail_path)
local num_files = count_directories(songlist_detail_path)

-- 打印文件个数
print("Number of files:", num_files)



