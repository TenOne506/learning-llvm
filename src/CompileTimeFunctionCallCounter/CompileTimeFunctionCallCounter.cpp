// CompileTimeFunctionCallCounter.cpp

// 包含必要的LLVM头文件
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

// 包含标准库头文件
#include <iostream>
#include <llvm/IR/Module.h>
#include <map>

// 使用LLVM和标准库的命名空间
using namespace llvm;
using FunctionCalls =
    std::map<const Function *,
             size_t>; // 定义一个映射，用于存储函数及其调用次数

// 定义一个LLVM Pass，用于分析函数调用次数
class CompileTimeFunctionCallCounter
    : public AnalysisInfoMixin<CompileTimeFunctionCallCounter> {
public:
  // 定义Pass的结果类型，即函数调用次数的映射
  using Result = FunctionCalls;

  // Pass的核心函数，分析模块中的函数调用
  Result run(Module &module, ModuleAnalysisManager &);

private:
  // 允许AnalysisInfoMixin访问Key
  friend struct AnalysisInfoMixin<CompileTimeFunctionCallCounter>;

  // 用于唯一标识此Pass的特殊键
  static AnalysisKey Key;
};

// 定义一个LLVM Pass，用于打印函数调用统计信息
class CompileTimeFunctionCallPrinter
    : public PassInfoMixin<CompileTimeFunctionCallPrinter> {
public:
  // Pass的核心函数，打印函数调用统计信息
  PreservedAnalyses run(Module &module, ModuleAnalysisManager &analysisManager);
};

// 初始化AnalysisKey
AnalysisKey CompileTimeFunctionCallCounter::Key;

// 实现CompileTimeFunctionCallCounter的run函数
CompileTimeFunctionCallCounter::Result
CompileTimeFunctionCallCounter::run(Module &module, ModuleAnalysisManager &) {
  Result functionCalls; // 存储函数调用次数的映射

  // 遍历模块中的每个函数
  for (const Function &function : module) {
    // 遍历函数中的每个基本块
    for (const BasicBlock &basicBlock : function) {
      // 遍历基本块中的每条指令
      for (const Instruction &instruction : basicBlock) {
        // 尝试将指令转换为函数调用指令
        const CallBase *functionCallInstruction =
            dyn_cast<CallBase>(&instruction);
        if (nullptr == functionCallInstruction) {
          // 如果指令不是函数调用，则跳过
          continue;
        }

        // 获取被调用的函数（仅适用于直接函数调用）
        const Function *calledFunction =
            functionCallInstruction->getCalledFunction();
        if (nullptr == calledFunction) {
          // 如果无法获取被调用的函数，则跳过
          continue;
        }

        // 查找或插入被调用函数的调用次数
        auto functionCallIterator = functionCalls.find(calledFunction);
        if (functionCalls.end() == functionCallIterator) {
          // 如果函数尚未在映射中，则插入并初始化调用次数为0
          functionCallIterator =
              functionCalls.insert(std::make_pair(calledFunction, 0)).first;
        }

        // 增加该函数的调用次数
        ++functionCallIterator->second;
      }
    }
  }

  // 返回函数调用次数的映射
  return functionCalls;
}

// 实现CompileTimeFunctionCallPrinter的run函数
PreservedAnalyses
CompileTimeFunctionCallPrinter::run(Module &module,
                                    ModuleAnalysisManager &analysisManager) {
  // 从ModuleAnalysisManager中获取CompileTimeFunctionCallCounter的分析结果
  auto functionCalls =
      analysisManager.getResult<CompileTimeFunctionCallCounter>(module);

  // 遍历并打印每个函数的调用次数
  for (auto &functionCall : functionCalls) {
    std::cout << "Function: " << functionCall.first->getName().str() << ", "
              << "called " << functionCall.second << " times" << std::endl;
  }

  // 假设我们没有修改IR代码，返回PreservedAnalyses::all()表示所有分析结果都保持不变
  return PreservedAnalyses::all();
}

// Pass注册函数
extern "C" LLVM_ATTRIBUTE_WEAK ::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION,          // LLVM插件API版本
          "CompileTimeFunctionCallPrinter", // Pass的名称
          LLVM_VERSION_STRING,              // Pass的版本
          [](PassBuilder &passBuilder) {
            // 1. 注册Pass以便通过"opt
            // -passes="compile-time-function-call-counter"运行
            passBuilder.registerPipelineParsingCallback(
                [&](StringRef name, ModulePassManager &passManager,
                    ArrayRef<PassBuilder::PipelineElement>) {
                  if (name == "compile-time-function-call-counter") {
                    // 如果Pass名称匹配，则将其添加到PassManager中
                    passManager.addPass(CompileTimeFunctionCallPrinter());
                    return true;
                  }

                  // 如果名称不匹配，返回false
                  return false;
                });

            // 2.
            // 注册分析Pass以便通过"analysisManager.getResult<CompileTimeFunctionCallCounter>(module)"使用
            passBuilder.registerAnalysisRegistrationCallback(
                [](ModuleAnalysisManager &analysisManager) {
                  analysisManager.registerPass(
                      [&]() { return CompileTimeFunctionCallCounter(); });
                });
          }};
}