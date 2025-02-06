// 引入LLVM头文件，用于定义pass manager、pass builder和pass plugin接口。
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

// 仅用于打印输出，iostream不是LLVM的一部分，但在这里用于标准输出。
#include <iostream>

using namespace llvm;

namespace {
// 定义一个自定义pass类MyPass，继承自PassInfoMixin<MyPass>模板类。
// PassInfoMixin提供了便捷的方法来管理pass的信息，如名称、版本等。
class MyPass : public PassInfoMixin<MyPass> {
public:
    // run()函数是pass的核心部分，它定义了pass在哪个级别（模块、函数等）运行，
    // 并接收对应的分析管理器（AnalysisManager）。这里的实现是在Function级别上操作。
    PreservedAnalyses run(Function &function, FunctionAnalysisManager &analysisManager) {
        // 使用std::cout打印当前处理的函数名。这里使用getName().str()获取函数名字符串表示。
        std::cout << "MyPass in function: " << function.getName().str() << std::endl;

        // 在此处可以添加对函数进行操作的代码。

        // 返回PreservedAnalyses::all()意味着该pass不会修改任何IR代码，
        // 因此所有之前的分析结果都仍然有效。
        return PreservedAnalyses::all();
    }
};
} // namespace

// 外部C函数，提供给LLVM用于加载插件时调用，返回一个PassPluginLibraryInfo结构体，
// 包含插件版本、名称和版本号等信息。
extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, // 插件API版本，确保与LLVM版本兼容。
        "MyPass",                // 插件名称。
        "v0.1",                  // 插件版本号。
        [](PassBuilder &passBuilder) {
            // 注册一个解析回调函数，当LLVM遇到指定名字的pipeline元素时被调用。
            passBuilder.registerPipelineParsingCallback(
                [](StringRef name, FunctionPassManager &passManager,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    // 如果pipeline中指定的名字为"my-pass"，则向passManager添加MyPass实例。
                    if (name == "my-pass") {
                        passManager.addPass(MyPass());
                        return true; // 成功处理了该pipeline元素。
                    }

                    return false; // 未处理该pipeline元素。
                });
        }};
}