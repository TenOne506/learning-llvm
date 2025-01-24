
//Arithmetic .cpp

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <vector>

int main(int argc, char *argv[]) {
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);

    llvm::Module *module = new llvm::Module("Hello BlOCK", context);

    // 添加一个带参数的函数
    std::vector<llvm::Type*> parameters(2,builder.getInt32Ty());
    llvm::Type *voidType = llvm::Type::getVoidTy(context);
    llvm::FunctionType *functionType = llvm::FunctionType::get(voidType, parameters,true);
    llvm::Function *function =
        llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage,
                               "Hello Function", module);
    llvm::BasicBlock *block = llvm::BasicBlock::Create(context, "emtry", function);
    // 设置函数参数
    function->getArg(0)->setName("Args-a");
    function->getArg(1)->setName("Args-b");

    builder.SetInsertPoint(block);

    // 添加一个全局变量
    module->getOrInsertGlobal("helloGlobalVariable", llvm::Type::getInt32Ty(context));
    llvm::GlobalVariable *globalVariable = module->getNamedGlobal("helloGlobalVariable");
    globalVariable->setLinkage(llvm::GlobalValue::CommonLinkage);
    globalVariable->setAlignment(llvm::MaybeAlign(4));
    //创建一个算术表达式
    llvm::Value* arg1 = function->getArg(0);
    llvm::ConstantInt* arg2 = builder.getInt32(3);
    llvm::Value* result = builder.CreateMul(arg1, arg2);
    // 添加一个返回值
    llvm::ConstantInt* zero = builder.getInt32(0);
    builder.CreateRet(zero);
    // 打印函数
    llvm::verifyFunction(*function);
    module->print(llvm::outs(), nullptr);
}
