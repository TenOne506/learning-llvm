/// HelloFunction.cpp

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Verifier.h>
int main() {
    llvm::LLVMContext context;

    // 创造一个模块
    llvm::Module *module = new llvm::Module("HelloModule", context);

    // 添加一个函数
    llvm::Type *voidType = llvm::Type::getVoidTy(context);
    llvm::FunctionType *functionType = llvm::FunctionType::get(voidType, false);
    llvm::Function *function =
        llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage,
                               "Hello Function", module);
    // 打印函数
    llvm::verifyFunction(*function);
    module->print(llvm::outs(), nullptr);
}



