cmake_minimum_required(VERSION 3.10)
project(HighPerformanceLogger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 fmt 库
find_package(fmt REQUIRED)

# 添加头文件目录
include_directories(include)

# 添加源文件
file(GLOB SOURCES "src/*.cpp")

# 创建可执行文件
add_executable(logger_test ${SOURCES})

# 添加线程库和 fmt 库
find_package(Threads REQUIRED)
target_link_libraries(logger_test Threads::Threads fmt::fmt) 
