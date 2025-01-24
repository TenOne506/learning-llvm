add_rules("mode.debug", "mode.release")
-- 设置编译器为 Clang
set_toolchains("clang")


-- -- 添加 LLVM 依赖
-- add_requires("llvm", {
--     system = true,  -- 使用系统安装的 LLVM
--     configs = {
--         links = {"LLVM"},  -- 链接 LLVM 库
--         includedirs = "/usr/local/include",  -- LLVM 头文件路径
--         linkdirs = "/usr/local/lib",  -- LLVM 库路径
--         syslinks = {"lrt","ldl","lm","lz","lzstd"}  -- 系统库依赖
--     }
-- })
-- 手动指定 LLVM 路径
-- local llvm_cflags = "/usr/local/include"
-- local llvm_ldflags = "/usr/local/lib"


-- local llvm_config_path = "/usr/local/bin/llvm-config"  -- 替换为你的实际路径
-- local llvm_cflags = os.iorun(llvm_config_path .. " --cxxflags")

-- local llvm_cflags = os.iorun(" llvm-config --cxxflags")
-- 定义一个函数来简化目标定义
function define_target(name, path)
    target(name)
        set_kind("binary")
        add_files(path .. "/*.cpp")  -- 添加源文件
        -- before_build(function (target)
        -- local llvm_cflags = os.iorun("llvm-config --cxxflags") -- 替换为你的实际路径
        -- local llvm_ldflags = os.run("llvm-config --ldflags --system-libs --libs core")
        -- -- add_cxflags(llvm_cflags)  -- 添加 LLVM 编译选项
      
        -- end)
         -- 在编译之前获取 llvm-config 标志
        before_build(function (target)
            -- 获取 LLVM 编译和链接标志
            local llvm_cflags = os.iorun("llvm-config --cxxflags")  -- 获取 C++ 编译标志
            local llvm_ldflags = os.iorun("llvm-config --ldflags --system-libs --libs core")  -- 获取链接标志
            local llvm_libs = os.iorun("llvm-config --libs all")  -- 获取所有必需的库
            -- Clean up library names by removing duplicate -l prefixes
            local cleaned_libs = {}
            for lib in string.gmatch(llvm_libs, "-l(%S+)") do
                table.insert(cleaned_libs, lib)
            end
            -- print("llvm_cflags: " .. llvm_cflags)
            -- print("llvm_ldflags: " .. llvm_ldflags)
            target:add("cxflags", llvm_cflags)  -- 添加 LLVM 编译标志
            target:add("ldflags", llvm_ldflags)  -- 添加 LLVM 链接标志
            target:add("links", cleaned_libs)  -- 添加清理后的 LLVM 库
            -- 手动指定核心库，尝试先链接核心库
            -- target:add("links", "LLVMCore", "LLVMSupport", "LLVMBitstreamReader", "LLVMRemarks")  
            -- 添加需要链接的核心库
        end)
        set_policy("check.auto_ignore_flags", false)
        -- add_packages("llvm")  -- 添加 LLVM 依赖
        add_cxflags("-std=c++17")  -- 设置 C++ 标准为 C++17
        -- add_cxflags(llvm_cflags)  -- 添加 LLVM 编译选项
        -- add_ldflags(llvm_ldflags)  -- 添加 LLVM 链接选项
        -- add_links("LLVM")  -- 手动添加 LLVM 库链接
        add_includedirs("/usr/local/include")  -- 手动添加 LLVM 头文件路径
        add_linkdirs("/usr/local/lib")  -- 手动添加 LLVM 库路径
end

-- 定义目标

define_target("Module", "src/LLVMModule")
define_target("Block", "src/LLVMBlock")
define_target("Function", "src/LLVMFunction")
define_target("Return", "src/LLVMReturn")
define_target("GlobalVar", "src/LLVMGlobalVar")
define_target("FunctionArgs", "src/LLVMFunctionArgs")
define_target("Arithmetic", "src/LLVMArithmetic")
