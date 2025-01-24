/// HelloBlock.cpp

#include <llvm/IR/Analysis.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include "llvm/IR/IRBuilder.h"
#include <llvm/IR/Verifier.h>
int main() {
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

    // 打印函数
    llvm::verifyFunction(*function);
    module->print(llvm::outs(), nullptr);
}