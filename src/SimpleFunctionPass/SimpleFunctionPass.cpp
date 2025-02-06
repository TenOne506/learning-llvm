#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class SimpleFunctionPass : public PassInfoMixin<SimpleFunctionPass> {
public:
  PreservedAnalyses run(Function &function, FunctionAnalysisManager &) {
    outs() << "Function: " << function.getName() << "\n";
    outs() << "    Instruction Count: " << function.getInstructionCount()
           << "\n";

    for (const auto &basicBlock : function) {
      outs() << "        BasicBlock: " << basicBlock.getName() << "\n";
    }

    outs() << "\n    Argument Count: " << function.arg_size() << "\n";
    for (const auto &arg : function.args()) {
      outs() << "        Argument: " << arg.getName() << "\n";
    }

    outs() << "\n    Function Type: \n";
    function.getFunctionType()->print(outs());
    outs() << "\n\n";

    return PreservedAnalyses::all();
  }
};
} // namespace

extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
  outs() << "SimpleFunctionPass plugin loaded!\n";
  return {LLVM_PLUGIN_API_VERSION, "SimpleFunctionPass", "v0.1",
          [](PassBuilder &passBuilder) {
            outs() << "SimpleFunctionPass registered!\n";
            passBuilder.registerPipelineParsingCallback(
                [](StringRef name, FunctionPassManager &passManager,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (name == "simple-function-pass") {
                    outs() << "SimpleFunctionPass added to pipeline!\n";
                    passManager.addPass(SimpleFunctionPass());
                    return true;
                  }
                  return false;
                });
          }};
}