# 包含当前目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 查找包 LLVM
# find_package(LLVM REQUIRED CONFIG)
# message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# 创建可执行文件 HelloFunction
add_executable(Function Function.cpp)

# 链接 LLVM 库
target_link_libraries(Function PRIVATE LLVMCore ${LLVM_LIBRARIES})
