#pragma once

// 引入必要的头文件
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/MemoryBuffer.h>

// Token 类表示词法分析器生成的词法单元
class Token {
public:
    // TokenType 枚举定义了所有可能的词法单元类型
    enum TokenType {
        kEOI,        // 输入结束 (End of Input)
        kUnknown,    // 未知词法单元
        kIdent,      // 标识符
        kNumber,     // 数字
        kComma,      // 逗号
        kColon,      // 冒号
        kPlus,       // 加号
        kMinus,      // 减号
        kStar,       // 星号
        kSlash,      // 斜杠
        kLeftParen,  // 左括号
        kRightParen, // 右括号
        kKeywordWith // 关键字 "with"
    };

private:
    TokenType type;       // 词法单元的类型
    llvm::StringRef text; // 词法单元的文本内容

public:
    // 获取词法单元的类型
    TokenType GetType() const {
        return type;
    }

    // 设置词法单元的类型
    void SetType(TokenType type) {
        this->type = type;
    }

    // 获取词法单元的文本内容
    llvm::StringRef GetText() const {
        return text;
    }

    // 设置词法单元的文本内容
    void SetText(llvm::StringRef text) {
        this->text = text;
    }

    // 判断当前词法单元是否为指定类型
    bool Is(TokenType inputtype) const {
        return type == inputtype;
    }

    // 判断当前词法单元是否为两种指定类型之一
    bool IsOneOf(TokenType type1, TokenType type2) const {
        return Is(type1) || Is(type2);
    }

    // 判断当前词法单元是否为多种指定类型之一（可变参数模板）
    template <typename... Ts>
    bool IsOneOf(TokenType type1, TokenType type2, Ts... types) const {
        return Is(type1) || IsOneOf(type2, types...);
    }
};

// Lexer 类负责将输入缓冲区解析为词法单元
class Lexer {
public:
    // 构造函数，接受一个字符串引用作为输入缓冲区
    Lexer(const llvm::StringRef &Buffer) {
        bufferStart = Buffer.begin(); // 初始化缓冲区的起始位置
        bufferPtr = bufferStart;      // 初始化当前解析位置
    }

    // 获取下一个词法单元
    void GetNext(Token &token);

private:
    const char *bufferStart; // 缓冲区的起始位置
    const char *bufferPtr;   // 当前解析位置

    // 初始化词法单元，设置其类型和文本内容
    void InitializeToken(Token &token, const char *tokenEnd, Token::TokenType type);
};