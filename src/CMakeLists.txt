find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
# 添加 LLVMFunction 子目录
add_subdirectory(LLVMFunction)

# 添加 LLVMModule 子目录
add_subdirectory(LLVMModule)

# 添加LLVMBlock 子目录
add_subdirectory(LLVMBlock)

# 添加LLVMGloobalVar 子目录
add_subdirectory(LLVMGlobalVar)

# 添加LLVMReturn 子目录
add_subdirectory(LLVMReturn)

# 添加LLVMReturn 子目录
add_subdirectory(LLVMFunctionArgs)

add_subdirectory(LLVMArithmetic)