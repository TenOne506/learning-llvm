#pragma once

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

// 前置声明
class AST;
class Expr;
class Factor;
class BinaryOp;
class WithDeclaration;

// ASTVisitor 类用于访问 AST 节点
class ASTVisitor {
public:
    // 访问顶级 AST 节点的虚函数，默认实现为空
    virtual void Visit(AST &) {
    }

    // 访问表达式节点的虚函数，默认实现为空
    virtual void Visit(Expr &) {
    }

    // 纯虚函数，子类必须提供具体实现
    virtual void Visit(Factor &) = 0;
    virtual void Visit(BinaryOp &) = 0;
    virtual void Visit(WithDeclaration &) = 0;
};

// 抽象基类 AST 定义了一个接口 Accept，所有派生类都必须实现这个接口
class AST {
public:
    virtual ~AST() {
    } // 虚析构函数确保通过基类指针删除派生类对象时正确调用析构函数

    // 接受一个 ASTVisitor 对象，并调用相应的 Visit 方法
    virtual void Accept(ASTVisitor &visitor) = 0;
};

// 表达式类继承自 AST，作为所有表达式的基类
class Expr : public AST {
public:
    Expr() {
    } // 默认构造函数
};

// 因子类继承自 Expr，表示表达式中的基本单元，如变量或数字
class Factor : public Expr {
public:
    enum ValueType { kIdent,
                     kNumber }; // 枚举类型，表示因子是标识符还是数值

private:
    ValueType type;        // 存储因子类型
    llvm::StringRef value; // 存储因子值

public:
    // 构造函数，初始化因子类型和值
    Factor(ValueType inType, llvm::StringRef inVal) :
        type(inType), value(inVal) {
    }

    // 获取因子类型
    ValueType GetType() {
        return type;
    }

    // 获取因子值
    llvm::StringRef GetVal() {
        return value;
    }

    // 实现 Accept 方法，调用 ASTVisitor 的相应 Visit 方法
    virtual void Accept(ASTVisitor &visitor) override {
        visitor.Visit(*this);
    }
};

// 二元操作类继承自 Expr，表示两个表达式之间的运算
class BinaryOp : public Expr {
public:
    enum Operator { kPlus,
                    kMinus,
                    kMultiple,
                    kDivide }; // 运算符枚举类型

private:
    Expr *left;  // 左侧表达式
    Expr *right; // 右侧表达式
    Operator op; // 操作符

public:
    // 构造函数，初始化操作符及两侧的表达式
    BinaryOp(Operator inOp, Expr *inLeftExpr, Expr *inRightExpr) :
        op(inOp), left(inLeftExpr), right(inRightExpr) {
    }

    // 获取左侧表达式
    Expr *GetLeft() {
        return left;
    }

    // 获取右侧表达式
    Expr *GetRight() {
        return right;
    }

    // 获取操作符
    Operator GetOperator() {
        return op;
    }

    // 实现 Accept 方法，调用 ASTVisitor 的相应 Visit 方法
    virtual void Accept(ASTVisitor &visitor) override {
        visitor.Visit(*this);
    }

    // 根据操作符返回相应的字符串表示
    std::string GetDisplayText() const {
        switch (op) {
        case kPlus: return "+";
        case kMinus: return "-";
        case kMultiple: return "*";
        case kDivide: return "/";
        default: break;
        }
        return "";
    }
};

// WithDeclaration 类用于表示 'with' 语句，包含一系列变量和一个表达式
class WithDeclaration : public AST {
    using VariableVector = llvm::SmallVector<llvm::StringRef, 8>; // 使用 SmallVector 存储变量名

public:
    // 构造函数，初始化变量列表和表达式
    WithDeclaration(VariableVector inVars, Expr *inExpr) :
        variables(inVars), expr(inExpr) {
    }

    // 返回变量列表的迭代器
    VariableVector::const_iterator begin() {
        return variables.begin();
    }
    VariableVector::const_iterator end() {
        return variables.end();
    }

    // 获取与 'with' 关联的表达式
    Expr *getExpr() {
        return expr;
    }

    // 实现 Accept 方法，调用 ASTVisitor 的相应 Visit 方法
    virtual void Accept(ASTVisitor &visitor) override {
        visitor.Visit(*this);
    }

private:
    VariableVector variables; // 存储变量名的向量
    Expr *expr;               // 'with' 语句中的表达式
};