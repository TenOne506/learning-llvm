#include "IRGenerator.h"

#include "llvm/ADT/StringMap.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

using namespace llvm;

namespace
{
    // IRGeneratorImplementation 类继承自 ASTVisitor，用于将 AST 转换为 LLVM IR
    class IRGeneratorImplementation : public ASTVisitor
    {
        Module* module; // 指向当前模块的指针
        IRBuilder<> builder; // 用于生成 IR 的构建器
        Type* voidType; // void 类型
        Type* int32Type; // 32 位整数类型
        Type* int8PtrType; // 8 位整数指针类型
        Type* int8PtrPtrType; // 指向 8 位整数指针的指针类型
        Constant* int32Zero; // 常量 0（32 位）

        Value* value; // 当前处理的值
        StringMap<Value*> nameMap; // 变量名到其 LLVM IR 值的映射

    public:
        // 构造函数初始化成员变量
        IRGeneratorImplementation(Module* inModule) :
            module(inModule),
            builder(inModule->getContext())
        {
            voidType = Type::getVoidTy(module->getContext());
            int32Type = Type::getInt32Ty(module->getContext());
            int8PtrType = Type::getInt8Ty(module->getContext());
            //int8PtrPtrType = int8PtrType->getPointerTo();
            int8PtrPtrType =PointerType::get(int8PtrType, 0);
            int32Zero = ConstantInt::get(int32Type, 0, true);
        }

        // 根据给定的 AST 生成中间代码表示
        void Generate(AST* tree)
        {
            // 创建 main 函数类型及其实现
            FunctionType* mainFunctionType = FunctionType::get(int32Type, {int32Type, int8PtrPtrType}, false);
            Function* mainFunction = Function::Create(mainFunctionType, GlobalValue::ExternalLinkage, "main", module);
            BasicBlock* basicBlock = BasicBlock::Create(module->getContext(), "entry", mainFunction);
            builder.SetInsertPoint(basicBlock);

            // 遍历 AST 并生成相应的 IR
            tree->Accept(*this);

            // 创建并调用 calc_write 函数
            FunctionType* calculatorWriteFunctionType = FunctionType::get(voidType, {int32Type}, false);
            Function* calculatorWriteFunction =
                Function::Create(calculatorWriteFunctionType, GlobalValue::ExternalLinkage, "calc_write", module);
            builder.CreateCall(calculatorWriteFunction, {value});

            // 返回 0 表示成功
            builder.CreateRet(int32Zero);
        }

        // 实现 Visit 方法来处理 Factor 节点
        void Visit(Factor& node) override
        {
            if (node.GetType() == Factor::kIdent)
            {
                // 如果是标识符，则从 nameMap 中查找对应的 LLVM IR 值
                value = nameMap[node.GetVal      ()];
            }
            else
            {
                // 否则，解析数值并创建对应的常量值
                int intValue;
                node.GetVal().getAsInteger(10, intValue);
                value = ConstantInt::get(int32Type, intValue, true);
            }
        };

        // 实现 Visit 方法来处理 BinaryOp 节点
        void Visit(BinaryOp& node) override
        {
            node.GetLeft()->Accept(*this); // 处理左侧子表达式
            Value* left = value;

            node.GetRight()->Accept(*this); // 处理右侧子表达式
            Value* right = value;

            // 根据运算符生成相应的 IR 指令
            switch (node.GetOperator())
            {
            case BinaryOp::kPlus:
                value = builder.CreateNSWAdd(left, right); // 加法
                break;

            case BinaryOp::kMinus:
                value = builder.CreateNSWSub(left, right); // 减法
                break;

            case BinaryOp::kMultiple:
                value = builder.CreateNSWMul(left, right); // 乘法
                break;

            case BinaryOp::kDivide:
                value = builder.CreateSDiv(left, right); // 除法
                break;
            }
        };

        // 实现 Visit 方法来处理 WithDeclaration 节点
        void Visit(WithDeclaration& node) override
        {
            // 创建 calc_read 函数类型及其实现
            FunctionType* calculatorReadFunctionType = FunctionType::get(int32Type, {int8PtrType}, false);
            Function* calculatorReadFunction =
                Function::Create(calculatorReadFunctionType, GlobalValue::ExternalLinkage, "calc_read", module);
            for (const auto& variable : node)
            {
                // 对于每个变量，创建一个全局字符串并调用 calc_read 函数获取其值
                Constant* strText = ConstantDataArray::getString(module->getContext(), variable);
                GlobalVariable* str = new GlobalVariable(*module,
                                                         strText->getType(),
                                                         /*isConstant=*/true,
                                                         GlobalValue::PrivateLinkage,
                                                         strText,
                                                         Twine(variable).concat(".str"));
                Value* ptr = builder.CreateInBoundsGEP(strText->getType(),str,{int32Zero, int32Zero}, "ptr");
                //builder.CreateInBoundsGEP(Type *Ty, Value *Ptr, ArrayRef<Value *> IdxList)
                CallInst* call = builder.CreateCall(calculatorReadFunction, {ptr});
                nameMap[variable] = call; // 将变量名与返回值关联起来
            }

            // 递归地处理与 'with' 关联的表达式
            node.getExpr()->Accept(*this);
        };
    };
} // namespace

// IRGenerator 类的 Generate 方法实现
void IRGenerator::Generate(AST* tree)
{
    LLVMContext context; // 创建 LLVM 上下文
    Module* module = new Module("calc.expr", context); // 创建新的模块
    IRGeneratorImplementation generator(module); // 创建 IRGeneratorImplementation 实例
    generator.Generate(tree); // 调用 Generate 方法生成 IR
    module->print(outs(), nullptr); // 输出生成的 IR
}