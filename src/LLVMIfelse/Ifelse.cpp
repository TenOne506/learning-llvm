///
// Test.c 为这个代码，生成IR
// int Test(int a)
// {
//     int b;
//     if (a > 33)
//     {
//         b = 66;
//     }
//     else
//     {
//         b = 77;
//     }

//     return b;
// }
///

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <vector>

int main() {
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);

    // 建立一个模块
    llvm::Module *module = new llvm::Module("Test.c", context);

    // 添加一个函数
    std::vector<llvm::Type *> paramter(1, builder.getInt32Ty());
    llvm::FunctionType *functiontype = llvm::FunctionType::get(builder.getInt32Ty(), paramter, true);
    llvm::Function *function = llvm::Function::Create(functiontype, llvm::GlobalValue::ExternalLinkage, "Test", module);
    
    //添加一个参数
    llvm::Value* arg = function->getArg(0);
    arg->setName("test_a");

    //未函数添加一些基本块
    llvm::BasicBlock* entryblock = llvm::BasicBlock::Create(context,"entry",function);
    llvm::BasicBlock* thenblock = llvm::BasicBlock::Create(context,"if.then",function);
    llvm::BasicBlock* elseblock = llvm::BasicBlock::Create(context,"if.else",function);
    llvm::BasicBlock* returnblock = llvm::BasicBlock::Create(context,"if.end",function);

    //填充entry块 i
    //int b;
    builder.SetInsertPoint(entryblock);
    llvm::Value *baddr = builder.CreateAlloca(builder.getInt32Ty(), nullptr);

    //填充entry块
    // if(a >33)
    llvm::ConstantInt* value = builder.getInt32(33);
    llvm::Value* cond = builder.CreateICmpSGT(arg, value,"compare.result");
    builder.CreateCondBr(cond, thenblock, elseblock);

    //填充ifthen block
    // b=66;
    builder.SetInsertPoint(thenblock);
    llvm::ConstantInt* value66 = builder.getInt32(66);
    builder.CreateStore(value66, baddr);
    builder.CreateBr(returnblock);

    //填充ifelse block
    //b=77
    builder.SetInsertPoint(elseblock);
    llvm::ConstantInt* value77 = builder.getInt32(77);
    builder.CreateStore(value77, baddr);
    builder.CreateBr(returnblock);

    //补充 end block
    //return b
    builder.SetInsertPoint(returnblock);
    llvm::Value* returnvalue = builder.CreateLoad(builder.getInt32Ty(),baddr,"return.value");

    //打印函数
    llvm::verifyFunction(*function);
    module->print(llvm::outs(), nullptr);
}