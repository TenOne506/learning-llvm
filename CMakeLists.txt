# 最小 CMake 版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(MyProject VERSION 1.0)
# 指定clang编译
# set(CMAKE_C_COMPILER   "clang")
# set(CMAKE_CXX_COMPILER "clang++")

# # 查找包 LLVM
# find_package(LLVM REQUIRED CONFIG)
# message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
# include_directories(${LLVM_INCLUDE_DIRS})
# add_definitions(${LLVM_DEFINITIONS})
# 添加调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
# 添加子目录
add_subdirectory(src)

# 如果需要安装目标，可以使用 install 命令
# install(TARGETS ... DESTINATION ...)