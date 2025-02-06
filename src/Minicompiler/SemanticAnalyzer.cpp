#include "SemanticAnalyzer.h"



#include "llvm/ADT/StringSet.h"
#include "llvm/Support/raw_ostream.h"

namespace {
// DeclarationChecker 类继承自 ASTVisitor，用于检查变量声明
class DeclarationChecker : public ASTVisitor {
public:
    // 构造函数初始化 hasError 为 false，并初始化作用域集合
    DeclarationChecker() :
        hasError(false) {
    }

    // 检查是否有错误发生
    bool HasError() {
        return hasError;
    }

    // 实现 Visit 方法来处理 Factor 节点，检查变量是否已声明
    void Visit(Factor &node) override {
        if (node.GetType() == Factor::kIdent) { // 如果是标识符类型的因子
            // 如果该变量未在当前作用域中声明，则报告错误
            if (scope.find(node.GetVal()) == scope.end()) {
                AddError(kNotDeclared, node.GetVal());
            }
        }
    }

    // 实现 Visit 方法来处理 BinaryOp 节点，递归检查左右子表达式
    void Visit(BinaryOp &node) override {
        if (node.GetLeft()) {
            node.GetLeft()->Accept(*this); // 递归检查左侧子表达式
        } else {
            hasError = true; // 左侧子表达式为空时设置错误标志
        }

        if (node.GetRight()) {
            node.GetRight()->Accept(*this); // 递归检查右侧子表达式
        } else {
            hasError = true; // 右侧子表达式为空时设置错误标志
        }
    }

    // 实现 Visit 方法来处理 WithDeclaration 节点，检查变量重复声明并递归检查关联表达式
    void Visit(WithDeclaration &node) override {
        for (auto variable : node) { // 遍历所有变量
            // 尝试插入变量到作用域集合中，如果插入失败（即变量已存在），则报告错误
            if (!scope.insert(variable).second) {
                AddError(kDeclaredTwice, variable);
            }
        }

        if (node.getExpr()) {
            node.getExpr()->Accept(*this); // 递归检查与 'with' 关联的表达式
        } else {
            hasError = true; // 表达式为空时设置错误标志
        }
    }

private:
    enum ErrorType { // 定义错误类型枚举
        kDeclaredTwice,
        kNotDeclared
    };

    // 添加错误信息
    void AddError(ErrorType errorType, llvm::StringRef variable) {
        llvm::errs() << "Variable " << variable << " "
                     << (errorType == kDeclaredTwice ? "already" : "not")
                     << " declared\n";
        hasError = true; // 设置错误标志
    }

private:
    llvm::StringSet<> scope; // 存储当前作用域中的变量名集合
    bool hasError;           // 标记是否有错误发生
};
} // namespace

// SemanticAnalyzer 类的 Analysis 方法实现
bool SemanticAnalyzer::Analysis(AST *tree) {
    if (!tree) { // 如果 AST 树为空，直接返回 false
        return false;
    }

    DeclarationChecker checker; // 创建 DeclarationChecker 对象
    tree->Accept(checker);      // 使用 checker 对象遍历整个 AST 树
    return checker.HasError(); // 如果没有错误发生，则返回 true；否则返回 false
}