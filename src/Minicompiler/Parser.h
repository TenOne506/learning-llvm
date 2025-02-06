#pragma once

#include "AST.h"
#include "Lexer.h"

#include "llvm/Support/raw_ostream.h"

// 解析器类，用于将词法分析器生成的token序列转换成抽象语法树（AST）
class Parser
{
public:
    // 构造函数，初始化解析器并获取第一个token
    Parser(Lexer& lexer) :
        lexer(lexer),
        hasError(false)
    {
        Advance(); // 获取下一个token
    }

    // 解析入口点，构建并返回AST
    AST* Parse();

    // 检查是否有解析错误发生
    bool HasError()
    {
        return hasError;
    }

private:
    // 解析计算表达式，具体实现取决于语言的语法规则
    AST* ParseCalc();
    
    // 解析表达式，通常包括加减运算等
    Expr* ParseExpr();
    
    // 解析项，通常包括乘除运算等
    Expr* ParseTerm();
    
    // 解析因子，如数字、变量或括号内的表达式
    Expr* ParseFactor();

    // 添加错误处理逻辑，报告意外的token
    void AddError()
    {
        llvm::errs() << "Unexpected: " << token.GetText() << "\n";
        hasError = true;
    }

    // 获取下一个token
    void Advance()
    {
        lexer.GetNext(token);
    }

    // 检查当前token是否是指定类型
    bool IsNextTokenOfType(Token::TokenType tokenType)
    {
        if (!token.Is(tokenType)) // 如果不是预期的token类型，则添加错误信息
        {
            AddError();
            return true;
        }
        return false;
    }

    // 尝试消耗一个特定类型的token，如果成功则前进到下一个token
    bool Consume(Token::TokenType tokenType)
    {
        if (IsNextTokenOfType(tokenType)) // 如果当前token不是预期类型，返回true表示消耗失败
        {
            return true;
        }
        Advance(); // 成功消耗，移动到下一个token
        return false;
    }

private:
    Lexer& lexer; // 词法分析器引用，用于获取token
    Token token; // 当前处理的token
    bool hasError; // 标记是否有解析错误发生
};