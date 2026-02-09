cmake_minimum_required(VERSION 3.10)
project(SimpleKV VERSION 0.1)

# 使用 C++17 或 C++20，符合你对现代 C++ 的偏好
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 开启常用警告，保持代码高质量
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# 包含头文件路径
include_directories(src)

# 自动查找 src 目录下的所有源文件 (为了起步简单，后续可手动指定)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

set(SOURCE_FILES
    src/main.cpp
    src/storage/Logger.cpp
    src/engine/KVStore.cpp
    src/server/TcpServer.cpp  # <--- 新增这一行
)

# 生成可执行文件
add_executable(simplekv ${SOURCES})

# 确保输出目录存在
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data)