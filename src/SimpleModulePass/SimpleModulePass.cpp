// SimpleModulePass.cpp

// 包含必要的LLVM头文件
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
// 包含标准输入输出库
#include <iostream>

// 使用LLVM和标准库的命名空间
using namespace llvm;
using std::cout;
using std::endl;

// 匿名命名空间，用于定义我们的Pass
namespace {
// 定义一个简单的模块Pass，继承自PassInfoMixin
class SimpleModulePass : public PassInfoMixin<SimpleModulePass> {
public:
  // Pass的核心函数，处理模块并输出相关信息
  PreservedAnalyses run(Module &module,
                        ModuleAnalysisManager &analysisManager) {
    // 输出模块的基本信息
    cout << "Module: " << module.getName().str() << endl;
    cout << "    ID: " << module.getModuleIdentifier() << endl;
    cout << "    Source File Name: " << module.getSourceFileName() << endl;
    cout << "    Instruction Count: " << module.getInstructionCount() << endl;

    // 输出模块中的所有全局变量
    cout << endl
         << "    Global Variable Count: " << module.global_size()
         << endl;
    for (const auto &globalVariable : module.global_values()) {
      cout << "        Global Variable: " << globalVariable.getName().str()
           << endl;
    }

    // 输出模块中的所有函数
    cout << endl
         << "    Function Count: " << module.getFunctionList().size() << endl;
    for (const auto &function : module) {
      cout << "        Function: " << function.getName().str() << endl;
    }

    // 假设我们没有修改IR代码，返回PreservedAnalyses::all()表示所有分析结果都保持不变
    return PreservedAnalyses::all();
  }
};
} // namespace

// 这是注册Pass的新方式
extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
  return {// LLVM插件API版本
          LLVM_PLUGIN_API_VERSION,
          // Pass的名称
          "SimpleModulePass",
          // Pass的版本
          "v0.1",
          // 注册Pass的回调函数
          [](PassBuilder &passBuilder) {
            passBuilder.registerPipelineParsingCallback(
                [](StringRef name, ModulePassManager &passManager,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  // 如果Pass的名称为"simple-module-pass"，则将其添加到PassManager中
                  if (name == "simple-module-pass") {
                    passManager.addPass(SimpleModulePass());
                    return true;
                  }

                  // 如果名称不匹配，返回false
                  return false;
                });
          }};
}