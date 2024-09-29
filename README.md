魔兽争霸3和服务器建立长连接的通讯服务
1.依赖 Ws2_32.lib


lua内置调用:

local ffi = require("ffi")
local storm = require ("jass.storm")
local japi = require("jass.japi")

ffi.cdef[[
    int init_socket(const char* ip, int port);
    int send_data(const char* data);
    const char* get_message();
    void close_socket();
]]

local tcp_path = ("%s\\tcp_socket.dll"):format(japi.GetMapName())
storm.save(tcp_path,storm.load("tcp_socket.tga"))
local is_lib, lib = pcall(ffi.load, tcp_path)
if not is_lib then
    print("加载失败")
    return
end

local result = lib.init_socket("127.0.0.1", 8080)
if result == 0 then
    print("连接成功")
    -- 每帧更新
          local msg = ffi.string(lib.get_message())
          if msg and msg ~= "" then
              print("收到数据: ", msg)
          end
    -- end
else
    print("连接失败")
end

-- 关闭长连接
-- lib.close_socket()
