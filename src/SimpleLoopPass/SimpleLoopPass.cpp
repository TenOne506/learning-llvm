// SimpleLoopPass.cpp

#include "llvm/IR/PassManager.h"      // 包含用于新式Pass管理的基础结构
#include "llvm/Passes/PassBuilder.h"  // 包含PassBuilder类，用于构建Pass管道
#include "llvm/Passes/PassPlugin.h"   // 包含Pass插件的相关声明
#include "llvm/Support/raw_ostream.h" // 包含LLVM支持的输出流功能

#include <iostream> // C++标准输入输出流库

using namespace llvm; // 使用llvm命名空间简化代码
using std::cout;      // 引入C++标准库中的cout
using std::endl;      // 引入C++标准库中的endl

namespace {
// 定义一个简单的循环Pass，继承自PassInfoMixin<SimpleLoopPass>
class SimpleLoopPass : public PassInfoMixin<SimpleLoopPass> {
public:
  // 实现run函数，该函数会在每次Pass运行时被调用
  PreservedAnalyses run(Loop &loop, LoopAnalysisManager &analysisManager,
                        LoopStandardAnalysisResults &analysisResults,
                        LPMUpdater &updater) {
    // 输出当前处理的循环名称
    cout << "Loop: " << loop.getName().str() << endl;

    // 输出子循环的数量，并遍历打印每个子循环的名称
    cout << endl << "    Sub-Loop Count: " << loop.getSubLoops().size() << endl;
    for (const auto &subLoop : loop.getSubLoops()) {
      cout << "        Sub-Loop: " << subLoop->getName().str() << endl;
    }

    // 输出当前循环中的基本块数量，并遍历打印每个基本块的名称
    cout << endl << "    Basic Block Count: " << loop.getNumBlocks() << endl;
    for (const auto &basicBlock : loop.getBlocks()) {
      cout << "        Basic Block: " << basicBlock->getName().str() << endl;
    }

    cout << endl << endl;

    // 假设我们没有对中间表示（IR）进行任何修改，返回PreservedAnalyses::all()
    return PreservedAnalyses::all();
  }
};
} // namespace

// 注册Pass到LLVM系统中，使得它可以被外部使用
extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "SimpleLoopPass", "v0.1",
          [](PassBuilder &passBuilder) {
            // 注册一个解析回调，当在命令行或通过其他方式指定pass管道时使用
            passBuilder.registerPipelineParsingCallback(
                [](StringRef name, LoopPassManager &passManager,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  // 如果传递的名字是"simple-loop-pass"，则将SimpleLoopPass添加到pass管理器中
                  if (name == "simple-loop-pass") {
                    passManager.addPass(SimpleLoopPass());
                    return true;
                  }

                  return false;
                });
          }};
}