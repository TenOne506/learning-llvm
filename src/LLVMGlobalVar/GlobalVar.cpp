
// HelloGlobalVar.cpp

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

int main(int argc, char *argv[]) {
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);

    llvm::Module *module = new llvm::Module("Hello BlOCK", context);

    // 添加一个函数
    llvm::Type *voidType = llvm::Type::getVoidTy(context);
    llvm::FunctionType *functionType = llvm::FunctionType::get(voidType, false);
    llvm::Function *function =
        llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage,
                               "Hello Function", module);
    llvm::BasicBlock *block = llvm::BasicBlock::Create(context, "emtry", function);
    builder.SetInsertPoint(block);

    // 添加一个全局变量
    module->getOrInsertGlobal("helloGlobalVariable", llvm::Type::getInt32Ty(context));
    llvm::GlobalVariable *globalVariable = module->getNamedGlobal("helloGlobalVariable");
    globalVariable->setLinkage(llvm::GlobalValue::CommonLinkage);
    globalVariable->setAlignment(llvm::MaybeAlign(4));
    // 打印函数
    llvm::verifyFunction(*function);
    module->print(llvm::outs(), nullptr);
}
